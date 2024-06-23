#include "pch.h"
#include "teams.hpp"

bool Teams::IsOnSameTeam(JNIEnv* Env, Player* LocalPlayer, Player* Player)
{
	if (!ModuleManager::GetModuleByName("Teams")->GetState()) {
		return false;
	}

	return LocalPlayer->IsOnSameTeam(Env, Player);
}
