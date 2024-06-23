#include "pch.h"
#include "handlestatusupdate.hpp"
#include <sdk/internal/register/envregister.hpp>
#include <cheat/module/module.hpp>

void HandleStatusUpdate::HandleStatusUpdateDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
{
	if (INSTANCE_DESTRUCT)
	{
		//EnvRegister::DestroyThreadEnv();
		return;
	}
	
	if (ModuleManager::SkipHandleStatusUpdate()) {
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

	/*static int RunOnce = [&Env, &Frame]()->int
	{
#ifdef SHOW_CONSOLE
		std::cout << "Detour -> RunTick" << std::endl;
		std::cout << "Env -> 0x" << Env << std::endl;
		std::cout << "This -> 0x" << HookManager->GetObjectParam(Frame, 0) << std::endl;
#endif
		return 0;
	}();*/

	const auto InstanceObject = std::make_shared<Player>(Env, Env->NewLocalRef(HookManager->GetObjectParam(Frame, 0)));
	const auto Byte = HookManager->get_primitive_param_at<jbyte>(Frame, 1);

	for (const auto& [Name, Mod] : ModuleManager::GetModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnHandleStatusUpdate(Env, InstanceObject, Byte, ShouldCancel);
		Env->PopLocalFrame(NULL);
	}

	for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnHandleStatusUpdate(Env, InstanceObject, Byte, ShouldCancel);
		Env->PopLocalFrame(NULL);
	}
}