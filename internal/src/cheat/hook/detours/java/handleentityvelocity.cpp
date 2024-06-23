#include "pch.h"
#include "handleentityvelocity.hpp"
#include <sdk/internal/register/envregister.hpp>
#include <cheat/module/module.hpp>

void HandleEntityVelocity::HandleEntityVelocityDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
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

	const auto InstanceObject = HookManager->GetObjectParam(Frame, 1);

	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	/*for (const auto& [Name, Mod] : ModuleManager::GetModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnHandleEntityVelocity(Env, InstanceObject, ShouldCancel);
		Env->PopLocalFrame(NULL);
	}

	for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnHandleEntityVelocity(Env, InstanceObject, ShouldCancel);
		Env->PopLocalFrame(NULL);
	}*/
}
