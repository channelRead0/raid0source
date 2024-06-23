#include "pch.h"
#include "attackentity.hpp"
#include <cheat/module/module.hpp>

#include "sdk/internal/register/envregister.hpp"

void AttackEntity::AttackEntityDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
{
	if (INSTANCE_DESTRUCT)
	{
		return;
	}

	if (!InstancePtr->GetJniEnv()) {
		return;
	}

	JNIEnv* Env = Thread->GetEnv();

	EnvRegister::RegisterThreadEnv(Env);
	const jobject AttackedEntity = jobject();

	/*for (const auto& [Name, Mod] : ModuleManager::GetModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnAttackEntityB(Env, AttackedEntity, ShouldCancel);
		Env->PopLocalFrame(NULL);
	}

	for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnAttackEntityB(Env, AttackedEntity, ShouldCancel);
		Env->PopLocalFrame(NULL);
	}*/

	return;
}
