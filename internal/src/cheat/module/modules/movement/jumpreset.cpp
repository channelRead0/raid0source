#include "pch.h"
#include "jumpreset.hpp"

void JumpReset::OnTick(JNIEnv* Env)
{
	Tick++;

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;
	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& GameSettings = Minecraft->GetGameSettingsPtr(Env);
	const auto& JumpKeybind = GameSettings->GetKeyBindJumpPtr(Env);
	const auto& SprintKeybind = GameSettings->GetKeyBindSprint(Env);
	const auto& ForwardKeybind = GameSettings->GetKeyBindForwardPtr(Env);
	const auto& BackKeybind = GameSettings->GetKeyBindBackPtr(Env);

	if (!Jumped || Tick <= 1) {
		return;
	}

	JumpKeybind->SetPressed(Env, JumpKeybind->KeyboardPressed(Env));
	SprintKeybind->SetPressed(Env, SprintKeybind->KeyboardPressed(Env));
	ForwardKeybind->SetPressed(Env, ForwardKeybind->KeyboardPressed(Env));
	BackKeybind->SetPressed(Env, BackKeybind->KeyboardPressed(Env));

	Jumped = false;
}

void JumpReset::OnPacketReceive(JNIEnv* Env, const std::shared_ptr<PacketReceiveData>& PacketData, bool* ShouldCancel)
{
	if (!this->GetState()) {
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& PacketInstance = PacketData->PacketInstance;
	const auto& GameSettings = ThreadClasses.Minecraft->GetGameSettingsPtr(Env);

	if (PacketInstance->IsVelocityPacket(Env) && !Jumped)
	{
		const auto VelocityPacket = std::make_shared<::VelocityPacket>(Env, PacketInstance->GetInstanceObject());
		if (VelocityPacket->GetEntityID(Env) != LocalPlayer->GetEntityID(Env)) {
			return;
		}


		if (!LocalPlayer->IsOnGround(Env)) {
			return;
		}

		if (Random.GenerateInt(0, 100) > Chance) {
			return;
		}

		GameSettings->GetKeyBindJumpPtr(Env)->SetPressed(Env, true);
		GameSettings->GetKeyBindSprint(Env)->SetPressed(Env, true);
		GameSettings->GetKeyBindForwardPtr(Env)->SetPressed(Env, true);
		GameSettings->GetKeyBindBackPtr(Env)->SetPressed(Env, false);

		Jumped = true;
		Tick = 0;
	}
}