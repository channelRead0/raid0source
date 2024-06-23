#include "pch.h"
#include "antibot.hpp"
#include <algorithm>
#include <concurrent_vector.h>
#include <concurrent_unordered_map.h>

void AntiBot::Run(JNIEnv* Env)
{
	Sleep(10000);
}

bool AntiBot::IsBot(JNIEnv* Env, World* World, Player* LocalPlayer, Player* Player)
{
	if (!ModuleManager::GetModuleByName("AntiBot")->GetState()) {
		return false;
	}

	if (Mode == HYPIXEL)
	{
		if (!Player->IsInvisible(Env) || !Player->IsOnGround(Env)) {
			return false;
		}

		const auto PlayerID = Player->GetEntityID(Env);
		const auto PlayerPos = Player->GetPos(Env);
		const auto PlayerLastTickPos = Player->GetLastTickPos(Env);
		const auto LocalPlayerPos = LocalPlayer->GetPos(Env);
		const auto PlayerName = Player->GetName(Env);
		const auto LocalPlayerLastTickPos = LocalPlayer->GetLastTickPos(Env);
		const auto LastPositionDeltaY = PlayerPos.Y - PlayerLastTickPos.Y;
		const auto PositionDeltaToLocalY = PlayerPos.Y - LocalPlayerPos.Y;
		auto IsBot = false;

		if (LastPositionDeltaY > 1.5F) {
			IsBot = true;
		}

		if (PositionDeltaToLocalY > 4.F && PositionDeltaToLocalY < 10.F && LastPositionDeltaY < 0.5F) {
			IsBot = true;
		}

		if (IsBot) {
			//std::cout << "Found Bot@" << PlayerName << std::endl;
		}

		return IsBot;
	}

	if (Mode == CUSTOM)
	{
		const auto PlayerID = Player->GetEntityID(Env);
		const auto PlayerPos = Player->GetPos(Env);
		const auto PlayerName = Player->GetName(Env);
		const auto PlayerTicksExisted = Player->GetTicksExisted(Env);
		const auto PlayerLastTickPos = Player->GetLastTickPos(Env);
		const auto LocalPlayerPos = LocalPlayer->GetPos(Env);
		const auto LocalPlayerLastTickPos = LocalPlayer->GetLastTickPos(Env);
		const auto LastPositionDeltaY = PlayerPos.Y - PlayerLastTickPos.Y;
		const auto PositionDeltaToLocalY = PlayerPos.Y - LocalPlayerPos.Y;
		auto IsBot = false;

		if (CheckBedwarsShops)
		{
			if (PlayerName == "SHOP" || PlayerName == "UPGRADES") {
				IsBot = true;
			}
		}

		if (CheckTicksExisted)
		{
			if (PlayerTicksExisted <= 0) {
				IsBot = true;
			}
		}

		if (CheckID)
		{
			if (PlayerID < 0) {
				IsBot = true;
			}
		}

		if (CheckName)
		{
			std::string Valid = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_";
			std::string PlayerName = Player->GetName(Env);

			for (int Index = 0; Index < PlayerName.length(); Index++)
			{
				auto Char = PlayerName.at(Index);
				if (!Valid.contains(Char)) 
				{
					IsBot = true;
					break;
				}
			}
		}

		if (IsBot) {
			//std::cout << "Found Bot@" << PlayerName << std::endl;
		}

		return IsBot;
	}

	return false;
}
