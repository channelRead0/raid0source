#include "pch.h"
#include "renderworldpass.hpp"
#include <cheat/module/module.hpp>
#include <sdk/internal/register/envregister.hpp>

void RenderWorldPass::RenderWorldPassDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
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

	const auto Pass = HookManager->get_primitive_param_at<jint>(Frame, 1);
	const auto PartialTicks = HookManager->get_primitive_param_at<jfloat>(Frame, 2);
	const auto FinishTimeNano = HookManager->get_primitive_param_at<jlong>(Frame, 3);

	//std::cout << "Pass: " << Pass << std::endl;

	for (const auto& [Name, Mod] : ModuleManager::GetModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnRenderWorldPass(Env, Pass, PartialTicks, FinishTimeNano);
		Env->PopLocalFrame(NULL);
	}

	for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnRenderWorldPass(Env, Pass, PartialTicks, FinishTimeNano);
		Env->PopLocalFrame(NULL);
	}
}
