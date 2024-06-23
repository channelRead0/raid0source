#include "pch.h"
#include "getmouseover.hpp"
#include <cheat/module/module.hpp>

#include "sdk/internal/register/envregister.hpp"

void GetMouseOver::GetMouseOverDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
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

	for (const auto& [Name, Mod] : ModuleManager::GetModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnGetMouseOver(Env);
		Env->PopLocalFrame(NULL);
	}

	for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnGetMouseOver(Env);
		Env->PopLocalFrame(NULL);
	}

	/*static int RunOnce = [&Env, &Frame]()->int
	{
#ifdef SHOW_CONSOLE
		std::cout << "Detour -> GetMouseOver" << std::endl;
		std::cout << "Env -> 0x" << Env << std::endl;
		std::cout << "This -> 0x" << HookManager->GetObjectParam(Frame, 0) << std::endl;
#endif
		return 0;
	}();*/

	return;
}
