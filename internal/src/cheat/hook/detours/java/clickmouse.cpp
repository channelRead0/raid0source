#include "pch.h"
#include "clickmouse.hpp"
#include <cheat/module/module.hpp>

#include "sdk/internal/register/envregister.hpp"

void ClickMouse::ClickMouseDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
{
	if (INSTANCE_DESTRUCT) 
	{
		//EnvRegister::DestroyThreadEnv();
		return;
	}

	if (!InstancePtr->GetJniEnv()) {
		return;
	}

	JNIEnv* Env = Thread->GetEnv();

	EnvRegister::RegisterThreadEnv(Env);

	/*for (const auto& [Name, Mod] : ModuleManager::GetModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnClickMouse(Env, ShouldCancel);
		Env->PopLocalFrame(NULL);
	}

	for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnClickMouse(Env, ShouldCancel);
		Env->PopLocalFrame(NULL);
	}*/

	return;
}
