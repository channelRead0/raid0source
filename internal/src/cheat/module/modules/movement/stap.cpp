#include "pch.h"
#include "stap.hpp"

void STap::OnTick(JNIEnv* Env)
{
	Tick++;
	static bool WasSet = false;
	static DWORD ReleaseTime = 0;

	auto ReleaseItem = [Env]() -> void
		{
			if (!WasSet) {
				return;
			}

			if (ReleaseTime > GetTickCount64()) {
				return;
			}

			GameClasses ThreadClasses;
			if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
				return;
			}

			const auto& Minecraft = ThreadClasses.Minecraft;
			const auto& GameSettings = Minecraft->GetGameSettingsPtr(Env);
			const auto& BackwardBind = GameSettings->GetKeyBindBackPtr(Env);

			BackwardBind->SetPressed(Env, false);
			WasSet = false;
		};

	auto UseItem = [Env](DWORD ItemReleaseTime) -> void
		{
			if (WasSet) {
				return;
			}

			ReleaseTime = ItemReleaseTime;

			GameClasses ThreadClasses;
			if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
				return;
			}

			const auto& Minecraft = ThreadClasses.Minecraft;
			const auto& GameSettings = Minecraft->GetGameSettingsPtr(Env);
			const auto& BackwardBind = GameSettings->GetKeyBindBackPtr(Env);

			BackwardBind->SetPressed(Env, true);
			WasSet = true;
		};

	if (!this->GetState())
	{
		ReleaseItem();
		return;
	}

	/*if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
	{
		if (WasSet)
		{
			InputPtr->ReleaseMouse(true);
			WasSet = false;
		}

		return;
	}

	if (!this->GetState())
	{
		if (WasSet)
		{
			
			<< "rmb unset" << std::endl;
			InputPtr->ReleaseMouse(true);
			WasSet = false;
		}

		return;
	}*/

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses))
	{
		ReleaseItem();
		return;
	}

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;
	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& MovingObjectPosition = Minecraft->GetObjectMouseOverPtr(Env);
	const auto& GameSettings = Minecraft->GetGameSettingsPtr(Env);
	const auto& EntityRenderer = Minecraft->GetEntityRendererPtr(Env);
	const auto& CurrentlyHeldItem = LocalPlayer->GetHeldItemPtr(Env);

	/*if (!CurrentlyHeldItem->IsSword(Env))
	{
		ReleaseItem();
		return;
	}*/

	if (!AttackedEntity)
	{
		ReleaseItem();
		return;
	}

	if (!WasSet && !LocalPlayer->IsSprinting(Env)) {
		return;
	}

	if (Tick == 1 /*&& AttackedEntity->GetHurtTime(Env) == 0*/)
	{
		/*if (!WasSet && !LocalPlayer->IsSprinting(Env)) {
			return;
		}*/

		const auto EntityDistance = AttackedEntity->GetPos(Env).Distance(LocalPlayer->GetPos(Env));

		if (Mode == PASSIVE && EntityDistance > MaintainDistance) {
			return;
		}

		//if (EntityDistance < RangeMin || EntityDistance > RangeMax)
		//{
		//	ReleaseItem();
		//	return;
		//}

		//if (Random.GenerateInt(0, 100) > Chance)
		//{
		//	ReleaseItem();
		//	return;
		//}

		const auto Proportion = Mode == PASSIVE ? std::clamp(static_cast<float>(EntityDistance) / (float)(EntityDistance - MaintainDistance), 0.F, 1.F) : std::clamp(static_cast<float>(EntityDistance) / 3.F, 0.F, 1.F);
		auto Delay = DelayMin + ((DelayMax - DelayMin) * Proportion);

		UseItem(GetTickCount64() + Delay);
	}

	if (Tick > 1)
	{
		ReleaseItem();
		return;
	}
}

void STap::OnHandleStatusUpdate(JNIEnv* Env, const std::shared_ptr<Player>& TargetInstance, jbyte Byte, bool* ShouldCancel)
{
	if (!this->GetState()) {
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& World = ThreadClasses.World;

	const auto& Timer = Minecraft->GetTimerPtr(Env);
	const auto& RenderManager = Minecraft->GetRenderManagerPtr(Env);
	const auto& EntityRenderer = Minecraft->GetEntityRendererPtr(Env);
	const auto& MovingObjectPosition = Minecraft->GetObjectMouseOverPtr(Env);

	if (MovingObjectPosition->IsMiss(Env)) {
		return;
	}

	const auto EntityMouseOver = std::make_shared<Player>(Env, MovingObjectPosition->EntityHit(Env));

	if (!TargetInstance->IsPlayer(Env)) {
		return;
	}

	if (TargetInstance->GetEntityID(Env) != EntityMouseOver->GetEntityID(Env)) {
		return;
	}

	AttackedEntity = EntityMouseOver;
	AttackedEntity->PromoteInstanceObject(Env);

	Tick = 0;
}