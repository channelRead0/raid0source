#include "pch.h"
#include "swingitem.hpp"
#include <cheat/module/module.hpp>

#include "sdk/internal/register/envregister.hpp"

void SwingItem::SwingItemDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
{
	if (ModuleManager::SkipSwingItem())
	{
		return;
	}

	if (INSTANCE_DESTRUCT)
	{
		return;
	}

	if (!InstancePtr->GetJniEnv()) {
		return;
	}

	JNIEnv* Env = Thread->GetEnv();

	EnvRegister::RegisterThreadEnv(Env);
	const jobject AttackedEntity = Env->NewLocalRef(HookManager->GetObjectParam(Frame, 1));

	/*for (const auto& [Name, Mod] : ModuleManager::GetModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnSwingItem(Env, ShouldCancel);
		Env->PopLocalFrame(NULL);
	}

	for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnSwingItem(Env, ShouldCancel);
		Env->PopLocalFrame(NULL);
	}*/

	return;
}
