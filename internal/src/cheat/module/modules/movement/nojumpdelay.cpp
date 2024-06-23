#include "pch.h"
#include "nojumpdelay.hpp"

void NoJumpDelay::Run(JNIEnv* Env)
{
	if (!this->GetState()) {
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto& LocalPlayer = ThreadClasses.Player;

	LocalPlayer->SetJumpTicks(Env, 0);
}
