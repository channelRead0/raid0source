#include "pch.h"
#include "framespoofer.hpp"
#include <sdk/game/minecraft/minecraft.hpp>


void FrameSpoofer::OnTick(JNIEnv* Env)
{
	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	if (!Env || !this->GetState()) {
		return;
	}

	auto Minecraft = ThreadClasses.Minecraft;

	int CurrrentFrames = Minecraft->GetDebugFPS(Env);

	static int LastFrameCount = 0;
	if (LastFrameCount == CurrrentFrames) {
		return;
	}

	int MultipliedFps = int(CurrrentFrames * 1.1F);

	Minecraft->SetDebugFPS(Env, MultipliedFps);
	LastFrameCount = MultipliedFps;
}
