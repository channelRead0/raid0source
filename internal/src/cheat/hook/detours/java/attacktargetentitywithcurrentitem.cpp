#include "pch.h"
#include "attacktargetentitywithcurrentitem.hpp"
#include <cheat/module/module.hpp>

#include "sdk/internal/register/envregister.hpp"

void AttackTargetEntityWithCurrentItem::AttackTargetEntityWithCurrentItemDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
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

	//LOCAL
	if (Env->IsSameObject(HookManager->GetObjectParam(Frame, 0), NULL)) {
		return;
	}

	//ATTACKED
	if (Env->IsSameObject(HookManager->GetObjectParam(Frame, 1), NULL)) {
		return;
	}

	const jobject AttackedEntity = Env->NewLocalRef(HookManager->GetObjectParam(Frame, 1));

	/*for (const auto& [Name, Mod] : ModuleManager::GetModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnAttackEntity(Env, AttackedEntity, ShouldCancel);
		Env->PopLocalFrame(NULL);
	}

	for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnAttackEntity(Env, AttackedEntity, ShouldCancel);
		Env->PopLocalFrame(NULL);
	}*/

	/*static int RunOnce = [&Env, &Frame]()->int
		{
#ifdef SHOW_CONSOLE
			
			<< "Detour -> AttackTargetEntityWithCurrentItem" << std::endl;
			std::cout << "Env -> 0x" << Env << std::endl;
			std::cout << "This -> 0x" << HookManager->GetObjectParam(Frame, 0) << std::endl;
#endif
			return 0;
		}();*/

	return;
}
