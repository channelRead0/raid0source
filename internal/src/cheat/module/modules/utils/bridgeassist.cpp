#include "pch.h"
#include "bridgeassist.hpp"

void BridgeAssist::Run(JNIEnv* Env)
{
	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;
	const auto& Minecraft = ThreadClasses.Minecraft;

	static bool IsSneaking = false;

	const auto& GameSettings = Minecraft->GetGameSettingsPtr(Env);
	const auto& KeyBindingSneak = GameSettings->GetKeyBindSneakPtr(Env);
	const auto& ItemStack = LocalPlayer->GetHeldItemPtr(Env);
	const auto& MovingObjectPosition = Minecraft->GetObjectMouseOverPtr(Env);
	const auto& CurrentBlockPosition = LocalPlayer->GetPos(Env) - Vector3<double>(0.0, 1.0, 0.0);
	const auto& CurrentBlock = World->GetBlock(Env, CurrentBlockPosition);
	const auto& NextBlockPosition = CurrentBlockPosition - Vector3<double>(0.0, 1.0, 0.0);
	const auto& NextBlock = World->GetBlock(Env, NextBlockPosition);

	if (!this->GetState())
	{
		if (IsSneaking)
		{
			KeyBindingSneak->SetPressed(Env, false);
			IsSneaking = false;
		}

		return;
	}

	static DWORD LastRmbTime = GetTickCount64();
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		LastRmbTime = GetTickCount64();
	}

	if (RmbOnly && (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000)) && GetTickCount64() - LastRmbTime > 500)
	{
		if (IsSneaking)
		{
			KeyBindingSneak->SetPressed(Env, false);
			IsSneaking = false;
		}

		return;
	}

	if (BackwardOnly && LocalPlayer->GetMoveForward(Env) > 0.F)
	{
		if (IsSneaking)
		{
			KeyBindingSneak->SetPressed(Env, false);
			IsSneaking = false;
		}

		return;
	}

	if (CheckPitch && (LocalPlayer->GetRotationPitch(Env) < 60.F))
	{
		if (IsSneaking)
		{
			KeyBindingSneak->SetPressed(Env, false);
			IsSneaking = false;
		}

		return;
	}

	if (BlocksOnly && !ItemStack->IsBlock(Env))
	{
		if (IsSneaking)
		{
			KeyBindingSneak->SetPressed(Env, false);
			IsSneaking = false;
		}

		return;
	}

	if (CurrentBlock->IsAir(Env) && NextBlock->IsAir(Env) && LocalPlayer->IsOnGround(Env) /*&& MovingObjectPosition->IsAimingBlock(Env)*/)
	{
		KeyBindingSneak->SetPressed(Env, true);
		IsSneaking = true;

		Sleep(Random.GenerateInt(Delay, Delay + 50));
		return;
	}

	else if (LocalPlayer->IsOnGround(Env))
	{
		KeyBindingSneak->SetPressed(Env, false);
		IsSneaking = false;
	}
}
