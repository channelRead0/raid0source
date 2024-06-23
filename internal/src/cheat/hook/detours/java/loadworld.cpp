#include "pch.h"
#include "loadworld.hpp"
#include <sdk/internal/register/envregister.hpp>
#include <cheat/module/module.hpp>

void LoadWorld::LoadWorldDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
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

	if (!Env) {
		return;
	}

	EnvRegister::RegisterThreadEnv(Env);

	for (const auto& [Name, Mod] : ModuleManager::GetModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnLoadWorld(Env, ShouldCancel);
		Env->PopLocalFrame(NULL);

		if (*ShouldCancel == true) {
			return;
		}
	}

	for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnLoadWorld(Env, ShouldCancel);
		Env->PopLocalFrame(NULL);

		if (*ShouldCancel == true) {
			return;
		}
	}
}
