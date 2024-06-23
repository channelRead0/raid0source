#include "pch.h"
#include "sprint.hpp"

void Sprint::Run(JNIEnv* Env)
{
	if (!this->GetState() || UI_OPEN) {
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& LocalPlayer = ThreadClasses.Player;

	const auto& GameSettings = Minecraft->GetGameSettingsPtr(Env);
	const auto& KeyBindSprint = GameSettings->GetKeyBindSprint(Env);
	const auto& KeyBindForward = GameSettings->GetKeyBindForwardPtr(Env);

	if (!KeyBindForward->GetPressed(Env)) {
		return;
	}

	if (DisableOnInvisible && LocalPlayer->IsInvisible(Env)) {
		return;
	}

	KeyBindSprint->SetPressed(Env, true);
}
