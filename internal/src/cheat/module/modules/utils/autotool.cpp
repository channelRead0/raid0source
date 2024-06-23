#include "pch.h"
#include "autotool.hpp"

void AutoTool::OnTick(JNIEnv* Env)
{
	if (!this->GetState() || UI_OPEN) {
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& Player = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;

	const auto& ObjectMouseOver = Minecraft->GetObjectMouseOverPtr(Env);
	const auto& PlayerController = Minecraft->GetPlayerControllerPtr(Env);
	const auto& Inventory = Player->GetInventoryPlayerPtr(Env);

	static bool IsAimingBlock = false;
	static DWORD LastBlockAimTime = 0;
	static int OriginalSlot = Inventory->GetCurrentSlot(Env);

	if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000)) 
	{
		IsAimingBlock = false;
		LastBlockAimTime = 0;
		return;
	}

	if (PlayerController->IsInCreativeMode(Env)) 
	{
		IsAimingBlock = false;
		LastBlockAimTime = 0;
		return;
	}

	if (!ObjectMouseOver->IsAimingBlock(Env)) 
	{
		IsAimingBlock = false;
		LastBlockAimTime = 0;
		return;
	}

	if (!IsAimingBlock && ObjectMouseOver->IsAimingBlock(Env))
	{
		IsAimingBlock = true;
		LastBlockAimTime = GetTickCount64();
	}

	if (IsAimingBlock && !ObjectMouseOver->IsAimingBlock(Env))
	{
		IsAimingBlock = false;
		LastBlockAimTime = 0;
	}

	if (GetTickCount64() - LastBlockAimTime < StartDelay) {
		return;
	}

	if (SneakingOnly && !Player->IsSneaking(Env)) {
		return;
	}

	const auto& BlockState = World->GetBlockState(Env, ObjectMouseOver->BlockPos(Env));
	const auto& Block = BlockState->GetBlockPtr(Env);

	float SpeedBest = 0.F;
	int BestSlot = OriginalSlot;

	for (int SlotIndex = 0; SlotIndex < 9; SlotIndex++)
	{
		const auto ItemStackInSlot = Inventory->ItemStackInSlot(Env, SlotIndex);

		if (ItemStackInSlot == nullptr || Env->IsSameObject(ItemStackInSlot->GetInstanceObject(), NULL)) {
			continue;
		}

		const auto BlockBreakSpeed = ItemStackInSlot->GetStrVsBlock(Env, Block->GetInstanceObject());

		if (BlockBreakSpeed > SpeedBest)
		{
			SpeedBest = BlockBreakSpeed;
			BestSlot = SlotIndex;
		}
	}

	static bool TimeSet = false;
	static DWORD SwitchTime = GetTickCount64();
	if (!TimeSet && Inventory->GetCurrentSlot(Env) != BestSlot) 
	{
		OriginalSlot = Inventory->GetCurrentSlot(Env);
		SwitchTime = GetTickCount64() + Random.GenerateInt(SwitchDelay, SwitchDelay + 20);
		TimeSet = true;
	}

	if (Inventory->GetCurrentSlot(Env) != BestSlot && GetTickCount64() > SwitchTime) 
	{
		Inventory->SetCurrentSlot(Env, BestSlot);
		TimeSet = false;
	}
}
