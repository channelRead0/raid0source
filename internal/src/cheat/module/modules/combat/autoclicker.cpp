#include "pch.h"
#include "autoclicker.hpp"

#include "sdk/game/input/input.hpp"
#include "sdk/game/minecraft/movingobjectposition.hpp"

#include "src/globals/utilities/maths/clicker/clicker.hpp"

void AutoClicker::Run(JNIEnv* Env)
{
	auto Start = GetTickCount64();

	if (!this->GetState()) {
		return;
	}

	if (OnlyClicking && !(GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
		return;
	}

	if (UI_OPEN){
		return;
	}

	if (GetForegroundWindow() != WINDOW_HANDLE) {
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;
	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& MovingObjectPosition = Minecraft->GetObjectMouseOverPtr(Env);
	const auto& EntityRenderer = Minecraft->GetEntityRendererPtr(Env);
	const auto& GuiScreen = Minecraft->GetCurrentScreenPtr(Env);
	const auto& PointedEntity = EntityRenderer->GetPointedEntityPtr(Env);
	const auto& ItemStack = LocalPlayer->GetHeldItemPtr(Env);
	const auto& Inventory = LocalPlayer->GetInventoryPlayerPtr(Env);
	const auto& PlayerController = Minecraft->GetPlayerControllerPtr(Env);

	if (!AllowInventory && GuiScreen->IsInMenu()) {
		return;
	}

	if (AllowInventory && GuiScreen->IsInMenu() && !(GetAsyncKeyState(VK_SHIFT) & 0x8000)) {
		return;
	}

	static DWORD LastBlockAimTime = 0;
	static bool IsAimingBlock = false;
	static bool BreakAssisted = false;
	if (!IsAimingBlock && MovingObjectPosition->IsAimingBlock(Env))
	{
		IsAimingBlock = true;
		LastBlockAimTime = GetTickCount64();
	}

	if (IsAimingBlock && !MovingObjectPosition->IsAimingBlock(Env))
	{
		IsAimingBlock = false;
		LastBlockAimTime = 0;
	}

	if (BreakBlocks && (IsAimingBlock && (GetTickCount64() - LastBlockAimTime > 350) || PlayerController->IsHittingBlock(Env)))
	{
		if (!BreakAssisted)
		{
			SendMessageW(WINDOW_HANDLE, 0x201, MK_LBUTTON, MAKELPARAM(0, 0));
			BreakAssisted = true;
		}

		Sleep(20);
		return;
	}

	if (BreakAssisted)
	{
		SendMessageW(WINDOW_HANDLE, 0x202, MK_LBUTTON, MAKELPARAM(0, 0));
		BreakAssisted = false;
	}

	bool IsItemWhitelisted = false;

	if (WhitelistSwords && ItemStack->IsSword(Env)) {
		IsItemWhitelisted = true;
	}

	if (WhitelistAxes && ItemStack->IsAxe(Env)) {
		IsItemWhitelisted = true;
	}

	if (WhitelistSticks && ItemStack->IsStick(Env)) {
		IsItemWhitelisted = true;
	}

	if (WhitelistPickAxe && ItemStack->IsPickAxe(Env)) {
		IsItemWhitelisted = true;
	}

	if (WhitelistFist && ItemStack->IsFist(Env)) {
		IsItemWhitelisted = true;
	}

	if (ItemWhitelist && !IsItemWhitelisted) {
		return;
	}

	if (LocalPlayer->GetHurtTime(Env) > 0) {
		ComboCount = 0;
	}

	if (GetTickCount64() - LastAttackTime > 1500.F) {
		ComboCount = 0;
	}

	if (PreventUnrefill && GuiScreen->IsInInventory(Env))
	{
		bool ContainsSpace = false;
		for (int SlotIndex = 0; SlotIndex < 9; SlotIndex++)
		{
			const auto SlotItemStack = Inventory->ItemStackInSlot(Env, SlotIndex);

			if (!SlotItemStack) {
				ContainsSpace = true;
			}

			if (!SlotItemStack->IsValid(Env)) {
				ContainsSpace = true;
			}
		}
		
		if (!ContainsSpace)
		{
			Sleep(1);
			return;
		}
	}

	if (AllowInventory && GuiScreen->IsInMenu() && (GetAsyncKeyState(VK_SHIFT) & 0x8000))
	{
		const auto ClickDelay = ClickerEnginePtr->GenerateDelay();
		const auto Duration = GetTickCount64() - Start;

		std::this_thread::sleep_for(std::chrono::milliseconds((int)ClickDelay));
		InputPtr->ClickMouse(false, Random.GenerateInt(1, 5));

		return;
	}

	if (NoHitDelay) {
		Minecraft->SetLeftClickCount(Env, 0);
	}

	auto DoSwing = [&]() -> void
	{
		bool IsSwingInProgress = LocalPlayer->IsSwingInProgress(Env);
		int SwingProgressInt = LocalPlayer->GetSwingProgressInt(Env);
		int ArmSwingAnimationEnd = LocalPlayer->GetArmSwingAnimationEnd(Env);

		if (!IsSwingInProgress || SwingProgressInt >= ArmSwingAnimationEnd / 2 || SwingProgressInt < 0)
		{
			LocalPlayer->SetSwingProgressInt(Env, -1);
			LocalPlayer->SetSwingInProgress(Env, true);
		}
	};

	if (HitSelect)
	{
		if (FavorCriticals && LocalPlayer->GetMotionY(Env) > 0.F)
		{
			if (FakeSwing) {
				DoSwing();
			}

			return;
		}

		if (FilterAir && MovingObjectPosition->IsMiss(Env) && Random.GenerateInt(0, 100) <= CancelRate)
		{
			if (FakeSwing) {
				DoSwing();
			}

			return;
		}

		if (MovingObjectPosition->IsAimingEntity(Env))
		{
			const auto TargetPlayer = std::make_shared<Player>(Env, MovingObjectPosition->EntityHit(Env));

			if (TargetPlayer->GetHurtTime(Env) < 0 + AttackWindow || TargetPlayer->GetHurtTime(Env) > 10 - AttackWindow) {
				goto HITSELECT;
			}

			return;
		}
	}

	HITSELECT:

	const auto ClickDelay = ClickerEnginePtr->GenerateDelay();
	const auto Duration = GetTickCount64() - Start;

	std::this_thread::sleep_for(std::chrono::milliseconds((int)ClickDelay));
	InputPtr->ClickMouse(false, Random.GenerateInt(1, 5));
}

void AutoClicker::OnHandleStatusUpdate(JNIEnv* Env, const std::shared_ptr<Player>& TargetInstance, jbyte Byte, bool* ShouldCancel)
{
	static int PreviousID;

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

	const auto& MovingObjectPosition = Minecraft->GetObjectMouseOverPtr(Env);

	if (!MovingObjectPosition->IsAimingEntity(Env)) {
		return;
	}

	const auto& Timer = Minecraft->GetTimerPtr(Env);
	const auto& RenderManager = Minecraft->GetRenderManagerPtr(Env);
	const auto& EntityRenderer = Minecraft->GetEntityRendererPtr(Env);
	const auto& EntityPointed = EntityRenderer->GetPointedEntityPtr(Env);

	const auto TargetPlayerMouseOver = std::make_shared<Player>(Env, MovingObjectPosition->EntityHit(Env));

	if (TargetInstance->GetEntityID(Env) == LocalPlayer->GetEntityID(Env)) {
		ComboCount = 0;
	}

	if (TargetInstance->GetEntityID(Env) == TargetPlayerMouseOver->GetEntityID(Env))
	{
		ComboCount++;

		if (TargetPlayerMouseOver->GetEntityID(Env) != PreviousID) 
		{
			PreviousID = TargetPlayerMouseOver->GetEntityID(Env);
			ComboCount = 1;
		}
	}
}
