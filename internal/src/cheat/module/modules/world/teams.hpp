#pragma once
#include "cheat/module/module.hpp"

class Teams : public Module
{
public:
	enum MODE
	{
		COLOR_BASED = 0,
		HYPIXEL = 1,
	};

	inline static int Mode = 0;
	inline static bool Enabled = true;

	static bool IsOnSameTeam(JNIEnv* Env, Player* LocalPlayer, Player* Player);
};