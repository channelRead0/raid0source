#pragma once
#include "cheat/module/module.hpp"
#include <concurrent_vector.h>
#include <concurrent_unordered_map.h>

class AntiBot : public Module
{
public:
	enum MODE
	{
		CUSTOM = 0,
		HYPIXEL = 1,
	};
	
	struct EntityData
	{
		bool IsBot;
		int TicksVisible;
	};

	inline static int Mode = CUSTOM;
	inline static bool ShowNotification = true;

	inline static bool CheckBedwarsShops = true;
	inline static bool CheckTicksExisted = true;
	inline static bool CheckName = true;
	inline static bool CheckID = true;

	virtual void Run(JNIEnv* Env) override;
	static bool IsBot(JNIEnv* Env, World* World, Player* LocalPlayer, Player* Player);
};