#pragma once
#include "../../module.hpp"
#include "concurrent_vector.h"
#include "cheat/hook/javahook.hpp"
#include <imgui/custom/custom.hpp>

struct BackTrackPacket
{
	std::shared_ptr<PacketReceiveData> Packet;
	DWORD QueueTime;

	BackTrackPacket(std::shared_ptr<PacketReceiveData> PacketData) : Packet(PacketData), QueueTime(GetTickCount64()) {};
};

struct BackTrackTarget
{
	std::mutex DataLock;

	std::shared_ptr<Player> Ptr = nullptr;

	AxisAlignedBB_T<double> BoundingBox;

	Vector3<double> Position;
	Vector3<double> LastTickPosition;

	Vector3<double> PositionRecord;

	int EntityID = -999;

	DWORD TargetTime = 0;

	BackTrackTarget(JNIEnv* Env, std::shared_ptr<Player> Target)
	{
		GameClasses ThreadClasses;
		if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses))
		{
			std::cerr << "Initialization Failed" << std::endl;
			return;
		}
		
		Target->PromoteInstanceObject(Env);

		this->Ptr = Target;
		this->Position = Target->GetPos(Env);
		this->PositionRecord = Target->GetPos(Env);
		this->LastTickPosition = Target->GetLastTickPos(Env);
		this->EntityID = Target->GetEntityID(Env);

		auto BoundsPre = AxisAlignedBB_T<double>
		{
			this->Position.X - 0.3,
			this->Position.Y,
			this->Position.Z - 0.3,
			this->Position.X + 0.3,
			this->Position.Y + 1.8,
			this->Position.Z + 0.3
		};

		const auto& Minecraft = ThreadClasses.Minecraft;
		const auto& Timer = Minecraft->GetTimerPtr(Env);
		const auto& RenderManager = Minecraft->GetRenderManagerPtr(Env);

		const auto RenderPartialTicks = Timer->GetRenderPartialTicks(Env);
		const auto RenderPosition = RenderManager->GetRenderPos(Env);

		this->BoundingBox = BoundsPre.GetBoundingBox3D(Position, LastTickPosition, RenderPosition, RenderPartialTicks);
		this->TargetTime = GetTickCount64();
	};

	void UpdatePosition(JNIEnv* Env, Vector3<double> PositionUpdate, float Lerp, bool IsRelative = false)
	{
		std::lock_guard<std::mutex> Lock(DataLock);

		GameClasses ThreadClasses;
		if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
			return;
		}

		if (IsRelative) 
		{
			this->Position = Position + PositionUpdate;
			//this->LastTickPosition = Position;
		}
		else {
			this->Position = PositionUpdate;
		}

		const auto& Minecraft = ThreadClasses.Minecraft;
		const auto& Timer = Minecraft->GetTimerPtr(Env);
		const auto& RenderManager = Minecraft->GetRenderManagerPtr(Env);

		const auto RenderPartialTicks = Timer->GetRenderPartialTicks(Env);
		const auto RenderPosition = RenderManager->GetRenderPos(Env);

		const auto Delta = Position - LastTickPosition;
		const auto Interpolated = LastTickPosition + (Delta * Lerp);

		auto InterpolatedBoundingBox = AxisAlignedBB_T<double>
		{
			Interpolated.X - 0.3,
			Interpolated.Y,
			Interpolated.Z - 0.3,
			Interpolated.X + 0.3,
			Interpolated.Y + 1.8,
			Interpolated.Z + 0.3
		};


		BoundingBox = InterpolatedBoundingBox.GetBoundingBox3D(Interpolated, LastTickPosition, RenderPosition, RenderPartialTicks);
		LastTickPosition = Interpolated;
	}

	void UpdatePositionTick(JNIEnv* Env, float Lerp)
	{
		std::lock_guard<std::mutex> Lock(DataLock);

		GameClasses ThreadClasses;
		if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
			return;
		}

		const auto& Minecraft = ThreadClasses.Minecraft;
		const auto& Timer = Minecraft->GetTimerPtr(Env);
		const auto& RenderManager = Minecraft->GetRenderManagerPtr(Env);

		const auto RenderPartialTicks = Timer->GetRenderPartialTicks(Env);
		const auto RenderPosition = RenderManager->GetRenderPos(Env);

		const auto Delta = Position - LastTickPosition;
		const auto Interpolated = LastTickPosition + (Delta * Lerp);

		auto InterpolatedBoundingBox = AxisAlignedBB_T<double>
		{
			Interpolated.X - 0.3,
			Interpolated.Y,
			Interpolated.Z - 0.3,
			Interpolated.X + 0.3,
			Interpolated.Y + 1.8,
			Interpolated.Z + 0.3
		};


		BoundingBox = InterpolatedBoundingBox.GetBoundingBox3D(Interpolated, LastTickPosition, RenderPosition, RenderPartialTicks);
		LastTickPosition = Interpolated;
	}

	std::shared_ptr<Player> GetTarget()
	{
		std::lock_guard<std::mutex> Lock(DataLock);
		return Ptr;
	}

	int GetEntityID()
	{
		std::lock_guard<std::mutex> Lock(DataLock);
		return EntityID;
	}

	DWORD GetTargetTime()
	{
		std::lock_guard<std::mutex> Lock(DataLock);
		return TargetTime;
	}

	AxisAlignedBB_T<double> GetLatestTargetBounds()
	{
		std::lock_guard<std::mutex> Lock(DataLock);
		return BoundingBox;
	}

	Vector3<double> GetPosition()
	{
		std::lock_guard<std::mutex> Lock(DataLock);
		return Position;
	}

	~BackTrackTarget()
	{
		std::lock_guard<std::mutex> Lock(DataLock);
		Ptr = nullptr;
	}
};

class BackTrack : public Module
{
private:
	std::atomic<DWORD> HitTime{ 0 };
	std::atomic<DWORD> StartTime{ 0 };
	std::atomic<DWORD> DisableTime{ 0 };

	std::atomic<int> PacketTicks{ 0 };
	std::atomic<int> ComboCount{ 0 };

	std::atomic<int> VirtualDelay{ 0 };

	std::atomic<bool> FlushNextTick{ false };
public:
	enum BACKTRACK_MODES
	{
		BACKTRACK_A = 0,
		BACKTRACK_B = 1,
	};

	inline static float DistanceMin = 1.F;
	inline static float DistanceMax = 3.F;
	inline static float LerpOffset = 0.01F;

	inline static bool DisableOnHit = true;
	inline static bool DisableOnDistance = true;
	inline static bool RenderServerRecord = true;
	
	inline static int Mode = BACKTRACK_A; // fully lagbased
	inline static int Delay = 50;
	inline static int Cooldown = 50;
	inline static int Sensitivity = 50;
	inline static int ComboThreshold = 2;

	inline static ImVec4 OutlineColor = Color(255, 255, 255, 255).GetColorVec4();
	inline static ImVec4 BoxColor = Color(255, 255, 255, 125).GetColorVec4();

	virtual void Run(JNIEnv* Env) override;
	virtual void OnTick(JNIEnv* Env) override;
	virtual void OnRespawn(JNIEnv* Env) override;
	virtual void OnRender(JNIEnv* Env) override;
	virtual void OnHandleStatusUpdate(JNIEnv* Env, const std::shared_ptr<Player>& TargetInstance, jbyte Byte, bool* ShouldCancel) override;
	virtual void OnPacketReceive(JNIEnv* Env, const std::shared_ptr<PacketReceiveData>& PacketData, bool* ShouldCancel) override;
};

class PacketQueueHandler
{
private:
	std::mutex PacketDataLock;
	std::vector<BackTrackPacket> PacketQueue = {};

	DWORD LastFlushTime = 0;
public:
	PacketQueueHandler() {}

	void QueuePacket(JNIEnv* Env, std::shared_ptr<PacketReceiveData> Packet)
	{
		std::lock_guard<std::mutex> Lock(PacketDataLock);

		Packet->ChannelHandlerContextInstance->PromoteInstanceObject(Env);
		Packet->NetworkManagerInstance->PromoteInstanceObject(Env);
		Packet->PacketInstance->PromoteInstanceObject(Env);

		PacketQueue.push_back({ Packet });
	}

	void UpdateQueueTick(JNIEnv* Env, float Delay)
	{
		std::lock_guard<std::mutex> Lock(PacketDataLock);

		GameClasses ThreadClasses;
		if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
			return;
		}

		if (PacketQueue.empty()) {
			return;
		}

		ModuleManager::SkipRead0Event = true;
		PacketQueue.erase(std::remove_if(PacketQueue.begin(), PacketQueue.end(), [&](BackTrackPacket BackTrackPacket)
		{
			const auto& [Packet, QueueTime] = BackTrackPacket;

			if ((GetTickCount64() - QueueTime) < Delay) {
				return false;
			}

			if (Packet->PacketInstance->IsTeamsPacket(Env))
			{
				const auto TeamsPacket = std::make_shared<::TeamsPacket>(Env, Packet->PacketInstance->GetInstanceObject());
				if (Env->IsSameObject(TeamsPacket->GetPlayers(Env), NULL)) { // Remove and dont processs
					return true;
				}
			}

			if (Packet->PacketInstance->IsSpawnPlayerPacket(Env))
			{
				const auto SpawnPlayerPacket = std::make_shared<::SpawnPlayerPacket>(Env, Packet->PacketInstance->GetInstanceObject());
				if (Env->IsSameObject(SpawnPlayerPacket->GetPlayer(Env), NULL)) { // Remove and dont processs
					return true;
				}
			}

			if (Packet->PacketInstance->IsScoreboardObjectivePacket(Env))
			{
				const auto ScoreboardObjectivePacket = std::make_shared<::ScoreboardObjectivePacket>(Env, Packet->PacketInstance->GetInstanceObject());
				if (Env->IsSameObject(ScoreboardObjectivePacket->func_149337_d(Env), NULL)) { // Remove and dont processs
					return true;
				}
			}

			Packet->NetworkManagerInstance->ChannelRead0(Env, Packet->ChannelHandlerContextInstance->GetInstanceObject(), Packet->PacketInstance->GetInstanceObject());

			Env->DeleteGlobalRef(Packet->NetworkManagerInstance->GetInstanceObject());
			Env->DeleteGlobalRef(Packet->ChannelHandlerContextInstance->GetInstanceObject());
			Env->DeleteGlobalRef(Packet->PacketInstance->GetInstanceObject());

			Packet->NetworkManagerInstance->UpdateInstanceObject(jobject());
			Packet->ChannelHandlerContextInstance->UpdateInstanceObject(jobject());
			Packet->PacketInstance->UpdateInstanceObject(jobject());

			return true;

		}), PacketQueue.end());
		ModuleManager::SkipRead0Event = false;
	}

	void ProcessAndClearQueue(JNIEnv* Env)
	{
		std::lock_guard<std::mutex> Lock(PacketDataLock);

		if (PacketQueue.empty()) {
			return;
		}

		ModuleManager::SkipRead0Event = true;
		for (const auto& [Packet, QueueTime] : PacketQueue)
		{
			bool DoFlush = true;
			if (Packet->PacketInstance->IsTeamsPacket(Env))
			{
				const auto TeamsPacket = std::make_shared<::TeamsPacket>(Env, Packet->PacketInstance->GetInstanceObject());
				if (Env->IsSameObject(TeamsPacket->GetPlayers(Env), NULL)) { // Remove and dont processs
					DoFlush = false;
				}
			}

			if (Packet->PacketInstance->IsSpawnPlayerPacket(Env))
			{
				const auto SpawnPlayerPacket = std::make_shared<::SpawnPlayerPacket>(Env, Packet->PacketInstance->GetInstanceObject());
				if (Env->IsSameObject(SpawnPlayerPacket->GetPlayer(Env), NULL)) { // Remove and dont processs
					DoFlush = false;
				}
			}

			if (Packet->PacketInstance->IsScoreboardObjectivePacket(Env))
			{
				const auto ScoreboardObjectivePacket = std::make_shared<::ScoreboardObjectivePacket>(Env, Packet->PacketInstance->GetInstanceObject());
				if (Env->IsSameObject(ScoreboardObjectivePacket->func_149337_d(Env), NULL)) { // Remove and dont processs
					DoFlush = false;
				}
			}

			if (!DoFlush) {
				continue;
			}

			Packet->NetworkManagerInstance->ChannelRead0(Env, Packet->ChannelHandlerContextInstance->GetInstanceObject(), Packet->PacketInstance->GetInstanceObject());

			Env->DeleteGlobalRef(Packet->NetworkManagerInstance->GetInstanceObject());
			Env->DeleteGlobalRef(Packet->ChannelHandlerContextInstance->GetInstanceObject());
			Env->DeleteGlobalRef(Packet->PacketInstance->GetInstanceObject());

			Packet->NetworkManagerInstance->UpdateInstanceObject(jobject());
			Packet->ChannelHandlerContextInstance->UpdateInstanceObject(jobject());
			Packet->PacketInstance->UpdateInstanceObject(jobject());
		}
		ModuleManager::SkipRead0Event = false;

		PacketQueue.clear();

		LastFlushTime = GetTickCount64();
	}

	void ClearQueue(JNIEnv* Env) //Only call on world change
	{
		std::lock_guard<std::mutex> Lock(PacketDataLock);

		if (PacketQueue.empty()) {
			return;
		}

		//ModuleManager::SkipRead0Event = true;
		for (const auto& [Packet, QueueTime] : PacketQueue)
		{
			Env->DeleteGlobalRef(Packet->NetworkManagerInstance->GetInstanceObject());
			Env->DeleteGlobalRef(Packet->ChannelHandlerContextInstance->GetInstanceObject());
			Env->DeleteGlobalRef(Packet->PacketInstance->GetInstanceObject());

			Packet->NetworkManagerInstance->UpdateInstanceObject(jobject());
			Packet->ChannelHandlerContextInstance->UpdateInstanceObject(jobject());
			Packet->PacketInstance->UpdateInstanceObject(jobject());
		}
		//ModuleManager::SkipRead0Event = false;

		PacketQueue.clear();

		LastFlushTime = GetTickCount64();
	}

	bool IsEmpty()
	{
		std::lock_guard<std::mutex> Lock(PacketDataLock);
		return PacketQueue.empty();
	}

	DWORD GetFlushTime()
	{
		std::lock_guard<std::mutex> Lock(PacketDataLock);
		return LastFlushTime;
	}
};

class TargetHandler
{
private:	
	std::mutex TargetLock;

	std::shared_ptr<BackTrackTarget> Target = nullptr;
	DWORD TargetTime = 0;
	DWORD ClearTime = 0;
public:
	TargetHandler() {}
	bool IsValid(JNIEnv* Env) 
	{
		std::lock_guard<std::mutex> Lock(TargetLock);
		return Target != nullptr && Target->GetTarget() != nullptr && !Env->IsSameObject(Target->GetTarget()->GetInstanceObject(), NULL) && !Target->GetTarget()->IsDead(Env);
	}

	void SetTarget(JNIEnv* Env, std::shared_ptr<Player> Target)
	{
		std::lock_guard<std::mutex> Lock(TargetLock);
		this->Target = std::make_shared<BackTrackTarget>(Env, Target);
		this->TargetTime = GetTickCount64();
	}

	DWORD GetTargetTime()
	{
		std::lock_guard<std::mutex> Lock(TargetLock);
		return TargetTime;
	}

	DWORD GetClearTime()
	{
		std::lock_guard<std::mutex> Lock(TargetLock);
		return ClearTime;
	}

	void ClearTarget()
	{
		std::lock_guard<std::mutex> Lock(TargetLock);
		this->Target = nullptr;
		this->ClearTime = GetTickCount64();
	}

	std::shared_ptr<BackTrackTarget>& GetTarget()
	{
		std::lock_guard<std::mutex> Lock(TargetLock);
		return this->Target;
	}
};

extern std::shared_ptr<PacketQueueHandler> PacketQueueHandlerPtr;
extern std::shared_ptr<TargetHandler> TargetHandlerPtr;