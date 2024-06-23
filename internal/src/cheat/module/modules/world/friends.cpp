#include "pch.h"
#include "friends.hpp"
#include <src/cheat/gui/notifications/notifications.hpp>

std::mutex ListMutex;
std::unordered_map<int, std::string> FriendList;

void Friends::Run(JNIEnv* Env)
{
	if (!this->GetState() || UI_OPEN)
	{
		Sleep(5);
		return;
	}

	GameClasses ThreadClasses;
	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses))
	{
		Sleep(1);
		return;
	}

	const auto& LocalPlayer = ThreadClasses.Player;
	const auto& World = ThreadClasses.World;
	const auto& Minecraft = ThreadClasses.Minecraft;
	const auto& EntityMouseOver = Minecraft->GetObjectMouseOverPtr(Env);
	const auto& EntityRenderer = Minecraft->GetEntityRendererPtr(Env);
	const auto& PointedEntity = EntityRenderer->GetPointedEntityPtr(Env);

	if (!(GetAsyncKeyState(VK_MBUTTON) & 0x8000)) 
	{
		Sleep(100);
		return;
	}

	while (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
	{
		Sleep(10);
	}

	if (!EntityMouseOver->IsAimingEntity(Env))
	{
		Sleep(100);
		return;
	}

	if (!PointedEntity->IsPlayer(Env))
	{
		Sleep(100);
		return;
	}

	const auto EntityID = PointedEntity->GetEntityID(Env);
	std::unique_lock<std::mutex> Lock(ListMutex);
	
	if (FriendList.contains(EntityID)) 
	{
		NotificationHandlerPtr->RegisterNotification("Friend Removed", (PointedEntity->GetName(Env) + " removed").data(), 2, 3000);
		FriendList.erase(EntityID);
		Sleep(10);
		return;
	}

	NotificationHandlerPtr->RegisterNotification("Friend Added", (PointedEntity->GetName(Env) + " added").data(), 2, 3000);

	FriendList[EntityID] = PointedEntity->GetName(Env);
	Sleep(10);
	return;
}

bool Friends::IsFriend(int EntityID)
{
	if (!ModuleManager::GetModuleByName("Friends")->GetState()) {
		return false;
	}

	std::unique_lock<std::mutex> Lock(ListMutex);

	return FriendList.contains(EntityID);
}
