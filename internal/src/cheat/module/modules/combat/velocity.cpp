#include "pch.h"
#include "velocity.hpp"

#include "sdk/game/minecraft/damagesource.hpp"

void Velocity::Run(JNIEnv* Env)
{
	if (!this->GetState()) {
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses))  {
		return;
	}

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;
	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& MovingObjectPosition = std::make_shared<::MovingObjectPosition>(Env, Minecraft->GetObjectMouseOver(Env));

	if (LiquidCheck && LocalPlayer->IsInWater(Env)) {
		return;
	}

	if (OnlyTargeting)
	{
		if (!MovingObjectPosition->IsAimingEntity(Env)) {
			return;
		}
	}

	if (CheckOnFire)
	{
		if (!LocalPlayer->IsPotionActive(Env, 12) && LocalPlayer->IsBurning(Env)) {
			return;
		}
	}

	if (OnlySprinting && !LocalPlayer->IsSprinting(Env)) {
		return;
	}

	if (LiquidCheck && LocalPlayer->IsInLava(Env)) {
		return;
	}

	if (Random.GenerateInt(0, 100) > Chance) {
		return;
	}

	/*if (VelocityMode == JUMP)
	{
		const auto GameSettings = Minecraft->GetGameSettingsPtr(Env);
		const auto KeyBindJump = GameSettings->GetKeyBindJumpPtr(Env);

		if (LocalPlayer->GetHurtTime(Env) != 10) 
		{
			Sleep(1);
			return;
		}

		KeyBindJump->SetPressed(Env, true);
		Sleep(Random.GenerateInt(30, 50));
		KeyBindJump->SetPressed(Env, false);

		while (LocalPlayer->GetHurtTime(Env) > 0)
		{
			Sleep(1);
		}

		Sleep(1);
		return;
	}*/

	if (VelocityMode == NORMAL)
	{
		if (LocalPlayer->GetHurtResistantTime(Env) != 20 - (UseDelay ? Delay : 0)) {
			return;
		}

		if (VelocityHorizontal != 100.F)
		{
			LocalPlayer->SetMotionX(Env, static_cast<float>(LocalPlayer->GetMotionX(Env) * (VelocityHorizontal / 100.F)));
			LocalPlayer->SetMotionZ(Env, static_cast<float>(LocalPlayer->GetMotionZ(Env) * (VelocityHorizontal / 100.F)));
		}

		if (VelocityVertical != 100.F) {
			LocalPlayer->SetMotionY(Env, static_cast<float>(LocalPlayer->GetMotionY(Env)* (VelocityVertical / 100.F)));
		}
	}

}

void Velocity::OnPacketReceive(JNIEnv* Env, const std::shared_ptr<PacketReceiveData>& PacketData, bool* ShouldCancel)
{
	//auto SetJumping = [Env](bool Jump)
	//{
	//	GameClasses ThreadClasses;
	//	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
	//		return;
	//	}

	//	const auto Minecraft = ThreadClasses.Minecraft;

	//	const auto GameSettings = Minecraft->GetGameSettingsPtr(Env);
	//	const auto KeyBindJump = GameSettings->GetKeyBindJumpPtr(Env);

	//	KeyBindJump->SetPressed(Env, Jump);
	//};

	auto IsAttackPacket = [Env, PacketData, ShouldCancel]() -> bool
	{
		GameClasses ThreadClasses;
		if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
			return false;
		}

		const auto LocalPlayer = ThreadClasses.Player;
		const auto Packet = PacketData->PacketInstance;
		if (!Packet->IsAttackPacket(Env)) {
			return false;
		}

		const auto AttackPacket = std::make_shared<::AttackPacket>(Env, Packet->GetInstanceObject());
		if (AttackPacket->GetEntityID(Env) != LocalPlayer->GetEntityID(Env)) {
			return false;
		}

		return AttackPacket->GetLogicOpCode(Env) == static_cast<jbyte>(2);
	};

	if (!this->GetState()) {
		return;
	}

	if (VelocityMode != PACKET /*&& VelocityMode != JUMP*/) {
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto Packet = PacketData->PacketInstance;
	const auto LocalPlayer = ThreadClasses.Player;

	if (OnlySprinting && !LocalPlayer->IsSprinting(Env)) {
		return;
	}

	if (Random.GenerateInt(0, 100) > Chance) {
		return;
	}

	//if (VelocityMode == JUMP)
	//{
	//	if (!IsAttackPacket()) {
	//		return;
	//	}
	//	
	//	if (!LocalPlayer->IsOnGround(Env)) {
	//		return;
	//	}

	//	ModuleManager::SkipRead0Event = true;

	//	PacketData->NetworkManagerInstance->ChannelRead0(Env, PacketData->ChannelHandlerContextInstance->GetInstanceObject(), PacketData->PacketInstance->GetInstanceObject());

	//	Env->DeleteGlobalRef(PacketData->ChannelHandlerContextInstance->GetInstanceObject());
	//	Env->DeleteGlobalRef(PacketData->NetworkManagerInstance->GetInstanceObject());
	//	Env->DeleteGlobalRef(PacketData->PacketInstance->GetInstanceObject());

	//	PacketData->NetworkManagerInstance->UpdateInstanceObject(jobject());
	//	PacketData->ChannelHandlerContextInstance->UpdateInstanceObject(jobject());
	//	PacketData->PacketInstance->UpdateInstanceObject(jobject());

	//	ModuleManager::SkipRead0Event = false;
	//	*ShouldCancel = true;
	//	/*if (LastAttackTime > 1000) {
	//		return;
	//	}*/

	//	/*if (!Packet->IsVelocityPacket(Env)) {
	//		return;
	//	}

	//	const auto VelocityPacket = std::make_shared<::VelocityPacket>(Env, Packet->GetInstanceObject());
	//	if (VelocityPacket->GetEntityID(Env) != LocalPlayer->GetEntityID(Env)) {
	//		return;
	//	}*/

	//	LocalPlayer->Jump(Env);
	//	std::cout << "Jump" << std::endl;

	//	return;
	//}

	if (VelocityMode == PACKET)
	{
		if (!Packet->IsVelocityPacket(Env)) {
			return;
		}

		const auto VelocityPacket = std::make_shared<::VelocityPacket>(Env, Packet->GetInstanceObject());
		if (VelocityPacket->GetEntityID(Env) != LocalPlayer->GetEntityID(Env)) {
			return;
		}

		if (LiquidCheck && LocalPlayer->IsInWater(Env)) {
			return;
		}

		if (CheckOnFire && !LocalPlayer->IsImmuneToFire(Env) && LocalPlayer->IsBurning(Env)) {
			return;
		}

		if (LiquidCheck && LocalPlayer->IsInLava(Env)) {
			return;
		}

		const auto MotionX = VelocityPacket->GetMotionX(Env);
		const auto MotionY = VelocityPacket->GetMotionY(Env);
		const auto MotionZ = VelocityPacket->GetMotionZ(Env);

		const auto MultiplierX = VelocityHorizontal / 100.F;
		const auto MultiplierY = VelocityVertical / 100.F;

		VelocityPacket->SetMotionX(Env, static_cast<int>(MotionX * MultiplierX));
		VelocityPacket->SetMotionY(Env, static_cast<int>(MotionY * MultiplierY));
		VelocityPacket->SetMotionZ(Env, static_cast<int>(MotionZ * MultiplierX));
	}
}
