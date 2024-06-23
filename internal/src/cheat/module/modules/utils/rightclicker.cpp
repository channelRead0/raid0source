#include "pch.h"
#include "rightclicker.hpp"

#include "sdk/game/input/input.hpp"

void RightClicker::Run(JNIEnv* Env)
{
	if (!this->GetState()) {
		return;
	}

	if (OnRmb && !(GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {
		return;
	}

	if (UI_OPEN) {
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
	const auto& GuiScreen = Minecraft->GetCurrentScreenPtr(Env);
	const auto& ItemStack = LocalPlayer->GetHeldItemPtr(Env);

	if (GuiScreen->IsInMenu()) {
		Sleep(5);
		return;
	}

	if (BlocksOnly && !ItemStack->IsBlock(Env)) {
		return;
	}

	InputPtr->ClickMouse(true, Random.GenerateInt(1, 5));

	const auto CPSms = 1000.F / TargetCPS;
	const auto CPSmsUpper = CPSms + (CPSms * 0.05F);
	const auto CPSmsLower = CPSms - (CPSms * 0.05F);
	const auto Delay = Random.GenerateInt(CPSmsLower, CPSmsUpper);

	std::this_thread::sleep_for(std::chrono::milliseconds(Delay));
}
