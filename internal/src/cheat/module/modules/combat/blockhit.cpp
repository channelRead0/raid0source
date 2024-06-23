#include "pch.h"
#include "blockhit.hpp"
#include "sdk/game/input/input.hpp"

void BlockHit::OnTick(JNIEnv* Env)
{
	Tick++;
	static bool WasSet = false;
	static DWORD ReleaseTime = 0;

	auto ReleaseItem = []() -> void
	{
		if (!WasSet) {
			return;
		}

		if (ReleaseTime > GetTickCount64()) {
			return;
		}

		InputPtr->ReleaseMouse(true);
		WasSet = false;
	};

	auto UseItem = [](DWORD ItemReleaseTime) -> void
	{
		if (WasSet) {
			return;
		}

		ReleaseTime = ItemReleaseTime;
		InputPtr->PressMouse(true);
		WasSet = true;
	};

	if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
	{
		ReleaseItem();
		return;
	}

	if (!this->GetState())
	{
		ReleaseItem();
		return;
	}

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

	if (!CurrentlyHeldItem->IsSword(Env)) 
	{
		ReleaseItem();
		return;
	}

	if (!AttackedEntity)
	{
		ReleaseItem();
		return;
	}

	if (Tick == 1)
	{
		if (Mode == PASSIVE)
		{
			const auto EntityDistance = AttackedEntity->GetPos(Env).Distance(LocalPlayer->GetPos(Env));

			const auto Proportion = std::clamp(static_cast<float>(EntityDistance) / 3.F, 0.F, 1.F);
			auto Delay = BlockHitDelayMin + ((BlockHitDelayMax - BlockHitDelayMin) * Proportion);

			UseItem(GetTickCount64() + Delay);
		}

		if (Mode == ACTIVE)
		{
			if (AttackedEntity->GetEntityID(Env) == LocalPlayer->GetEntityID(Env))
			{
				const auto Proportion = 1.F;
				auto Delay = BlockHitDelayMin + ((BlockHitDelayMax - BlockHitDelayMin) * Proportion);

				UseItem(GetTickCount64() + Delay);

				return;
			}

			const auto EntityDistance = AttackedEntity->GetPos(Env).Distance(LocalPlayer->GetPos(Env));

			const auto Proportion = std::clamp(static_cast<float>(EntityDistance) / 3.F, 0.F, 1.F);
			auto Delay = BlockHitDelayMin + ((BlockHitDelayMax - BlockHitDelayMin) * Proportion);

			UseItem(GetTickCount64() + Delay);
		}
	}

	if (Tick > 1)
	{
		ReleaseItem();
		return;
	}
}

void BlockHit::OnHandleStatusUpdate(JNIEnv* Env, const std::shared_ptr<Player>& TargetInstance, jbyte Byte, bool* ShouldCancel)
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

	const auto MovingObjectPosition = Minecraft->GetObjectMouseOverPtr(Env);

	if (MovingObjectPosition->IsMiss(Env)) {
		return;
	}

	auto EntityMouseOver = std::make_shared<Player>(Env, MovingObjectPosition->EntityHit(Env));

	if (Mode == PASSIVE)
	{
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

	if (Mode == ACTIVE)
	{
		if (!TargetInstance->IsPlayer(Env)) {
			return;
		}

		const auto EntityID = TargetInstance->GetEntityID(Env);
		if (EntityID != EntityMouseOver->GetEntityID(Env) && EntityID != LocalPlayer->GetEntityID(Env)) {
			return;
		}

		AttackedEntity = EntityMouseOver;
		AttackedEntity->PromoteInstanceObject(Env);

		Tick = 0;
	}
}