#include "pch.h"
#include "fastplace.hpp"

void FastPlace::Run(JNIEnv* Env)
{
	if (!this->GetState() || UI_OPEN) {
		return;
	}

	//if (OnRmb && !(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
	//{
	//	Sleep(10);
	//	return;
	//}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
		return;
	}

	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& Player = ThreadClasses.Player;
	const auto& HeldItemStack = Player->GetHeldItemPtr(Env);
	
	if (BlocksOnly && !HeldItemStack->IsBlock(Env)) {
		return;
	}

	const auto PlaceDelay = Random.GenerateInt(0, TickDelay);

	Minecraft->SetRightClickDelayTimer(Env, Randomize ? PlaceDelay : TickDelay);
}
