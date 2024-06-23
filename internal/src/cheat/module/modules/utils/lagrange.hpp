#pragma once
#include "../../module.hpp"
#include "concurrent_vector.h"
#include <imgui/custom/custom.hpp>

class PacketReceiveQueueHandler
{
private:
	std::mutex PacketDataLock;
	std::vector<std::shared_ptr<PacketSendData>> PacketQueue = {};

	DWORD LastFlushTime = 0;
	DWORD LastQueueTime = 0;

	Vector3<double> LastPosition = {};
	Vector3<double> LastServerPosition = {};

	bool RenderPosition = false;
public:
	PacketReceiveQueueHandler() {}

	void QueuePacket(JNIEnv* Env, std::shared_ptr<PacketSendData> Packet)
	{
		std::lock_guard<std::mutex> Lock(PacketDataLock);

		GameClasses ThreadClasses;
		if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
			return;
		}

		const auto& LocalPlayer = ThreadClasses.Player;

		if (PacketQueue.empty())
		{
			LastQueueTime = GetTickCount64();
			LastPosition = LocalPlayer->GetPos(Env);
			LastServerPosition = LocalPlayer->GetPos(Env);
		}

		Packet->NetHandlerInstance->PromoteInstanceObject(Env);
		Packet->PacketInstance->PromoteInstanceObject(Env);

		PacketQueue.push_back(Packet);

		RenderPosition = true;
	}

	DWORD GetLastQueueTime()
	{
		std::lock_guard<std::mutex> Lock(PacketDataLock);
		return LastQueueTime;
	}

	DWORD GetLastFlushTime()
	{
		std::lock_guard<std::mutex> Lock(PacketDataLock);
		return LastFlushTime;
	}

	void ProcessAndClearQueue(JNIEnv* Env, std::shared_ptr<NetHandlerPlayClient> NetHandler)
	{
		//std::cout << "Processing" << std::endl;
		std::lock_guard<std::mutex> Lock(PacketDataLock);

		GameClasses ThreadClasses;
		if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
			return;
		}

		const auto& LocalPlayer = ThreadClasses.Player;
		const auto& World = ThreadClasses.World;
		const auto& Minecraft = ThreadClasses.Minecraft;
		const auto& NetworkManager = LocalPlayer->GetSendQueuePtr(Env)->GetNetworkManager(Env);

		if (PacketQueue.empty()) 
		{
			LastServerPosition = LocalPlayer->GetPos(Env);
			return;
		}

		LastQueueTime = 0;
		LastFlushTime = GetTickCount64();

		ModuleManager::SkipAddToSendQueueEvent = true;
		for (const auto& Packet : PacketQueue)
		{
			if (Packet->PacketInstance->IsPlayerPacket(Env))
			{
				const auto PlayerPacket = std::make_shared<::PacketPlayer>(Env, Packet->PacketInstance->GetInstanceObject());
				LastServerPosition = PlayerPacket->GetPosition(Env);
			}

			Packet->NetHandlerInstance->AddToSendQueue(Env, Packet->PacketInstance->GetInstanceObject());

			Env->DeleteGlobalRef(Packet->NetHandlerInstance->GetInstanceObject());
			Env->DeleteGlobalRef(Packet->PacketInstance->GetInstanceObject());

			Packet->NetHandlerInstance->UpdateInstanceObject(jobject());
			Packet->PacketInstance->UpdateInstanceObject(jobject());
		}
		ModuleManager::SkipAddToSendQueueEvent = false;

		PacketQueue.clear();

		RenderPosition = false;
	}

	void ClearQueue(JNIEnv* Env)
	{
		PacketQueue.clear();
		RenderPosition = false;
	}

	bool ShouldRender()
	{
		std::lock_guard<std::mutex> Lock(PacketDataLock);
		return RenderPosition;
	}

	Vector3<double> GetLastPosition()
	{
		std::lock_guard<std::mutex> Lock(PacketDataLock);
		return LastPosition;
	}

	Vector3<double> GetLastServerPosition()
	{
		std::lock_guard<std::mutex> Lock(PacketDataLock);
		return LastServerPosition;
	}
};

class LagRange : public Module
{
private:
public:
	inline static float ActivationDistance = 7.F;
	inline static bool RenderServerRecord = true;
	inline static int Delay = 500;
	inline static int Cooldown = 500;

	inline static ImVec4 OutlineColor = Color(255, 255, 255, 255).GetColorVec4();
	inline static ImVec4 BoxColor = Color(255, 255, 255, 125).GetColorVec4();

	virtual void OnTick(JNIEnv* Env) override;
	virtual void OnPacketSend(JNIEnv* Env, const std::shared_ptr<PacketSendData>& PacketData, bool* ShouldCancel) override;
	virtual void OnPacketReceive(JNIEnv* Env, const std::shared_ptr<PacketReceiveData>& PacketData, bool* ShouldCancel) override;
	virtual void OnRespawn(JNIEnv* Env) override;
	virtual void OnRender(JNIEnv* Env) override;
};

extern std::shared_ptr<PacketReceiveQueueHandler> PacketReceiveQueueHandlerPtr;