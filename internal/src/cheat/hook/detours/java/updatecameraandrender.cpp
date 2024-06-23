#include "pch.h"
#include "updatecameraandrender.hpp"
#include <sdk/internal/register/envregister.hpp>

void UpdateCameraAndRender::UpdateCameraAndRenderDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
{
	if (INSTANCE_DESTRUCT)
	{
		//EnvRegister::DestroyThreadEnv();
		return;
	}

	if (!InstancePtr->GetJniEnv()) {
		return;
	}

	if (ModuleManager::SkipSetAngles()) {
		return;
	}

	JNIEnv* Env = Thread->GetEnv();

	EnvRegister::RegisterThreadEnv(Env);

	float Yaw = HookManager->get_primitive_param_at<jfloat>(Frame, 1);
	float Pitch = HookManager->get_primitive_param_at<jfloat>(Frame, 2);

	/*static int RunOnce = [&Env, &Frame]()->int
	{
#ifdef SHOW_CONSOLE
		std::cout << "Detour -> UpdateCameraAndRender" << std::endl;
		std::cout << "Env -> 0x" << Env << std::endl;
		std::cout << "This -> 0x" << HookManager->GetObjectParam(Frame, 0) << std::endl;
#endif
		return 0;
	}();*/

	for (const auto& [Name, Mod] : ModuleManager::GetModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnSetAngles(Env, Yaw, Pitch, ShouldCancel);
		Env->PopLocalFrame(NULL);

		if (*ShouldCancel == true) {
			return;
		}
	}

	for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnSetAngles(Env, Yaw, Pitch, ShouldCancel);
		Env->PopLocalFrame(NULL);

		if (*ShouldCancel == true) {
			//std::cout << "Cancelled" << std::endl;
			return;
		}
	}

	return;
}
