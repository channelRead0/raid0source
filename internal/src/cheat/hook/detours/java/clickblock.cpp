#include "pch.h"
#include "clickblock.hpp"
#include <cheat/module/module.hpp>

#include "sdk/internal/register/envregister.hpp"

void ClickBlock::ClickBlockDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
{
	if (INSTANCE_DESTRUCT)
	{
		return;
	}

	if (ModuleManager::SkipSwingItem())
	{
		return;
	}

	if (!InstancePtr->GetJniEnv()) {
		return;
	}

	JNIEnv* Env = Thread->GetEnv();

	EnvRegister::RegisterThreadEnv(Env);
	
	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	/*const auto& LocalPlayer = ThreadClasses.Player;

	ModuleManager::SkipSwingItemEvent = true;
	LocalPlayer->SwingItem(Env);
	ModuleManager::SkipSwingItemEvent = false;*/

	return;
}
