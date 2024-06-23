#include "pch.h"
#include "onswapbuffers.hpp"
#include "wglswapbuffers.hpp"

#include "cheat/module/modules/render/render.hpp"
#include "sdk/game/minecraft/activerenderinfo.hpp"
#include "sdk/internal/register/envregister.hpp"

decltype(OnSwapBuffersHook::OriginalSwapBuffers) OnSwapBuffersHook::OriginalSwapBuffers;
long OnSwapBuffersHook::SwapBuffersDetour(JNIEnv* Env, jclass CallingClass, jobject CallingObject)
{
	if (INSTANCE_DESTRUCT)
	{
		//EnvRegister::DestroyThreadEnv(); 
		return OnSwapBuffersHook::OriginalSwapBuffers(Env, CallingClass, CallingObject);
	}

	static auto LastFrameTime = std::chrono::high_resolution_clock::now();
	const auto CurrentFrameTime = std::chrono::high_resolution_clock::now();
	const float Delta = duration_cast<std::chrono::milliseconds>(CurrentFrameTime - LastFrameTime).count();

	DeltaTime = static_cast<float>(Delta / 1000.F);
	LastFrameTime = CurrentFrameTime;

	GameClasses ThreadClasses;

	IN_GAME = ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses);

	if (!IN_GAME) {
		return OnSwapBuffersHook::OriginalSwapBuffers(Env, CallingClass, CallingObject);
	}

	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& CurrentScreen = Minecraft->GetCurrentScreenPtr(Env);

	WglSwapBuffersHook::InChat = CurrentScreen->IsInChat(Env);

	return OnSwapBuffersHook::OriginalSwapBuffers(Env, CallingClass, CallingObject);
}
