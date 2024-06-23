#include "pch.h"
#include "blink.hpp"
#include <concurrent_vector.h>
#include <src/cheat/hook/detours/java/channelread0.hpp>
#include <gl/GL.h>
#include "sdk/game/minecraft/activerenderinfo.hpp"

//void Blink::Run(JNIEnv* Env)
//{
//	static long Timer = GetTickCount64();
//	static bool PreviousState = false;
//
//	if (!this->GetState() || UI_OPEN) 
//	{
//		DoBlink = false;
//		PreviousState = false;
//
//		return;
//	}
//
//	if (this->GetState() && !UI_OPEN && !PreviousState)
//	{
//		const auto Module = ModuleManager::GetModuleByName("BackTrack");
//		Module->Disable();
//
//		Timer = GetTickCount64();
//		PreviousState = true;
//	}
//
//	GameClasses ThreadClasses;
//	if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) 
//	{
//		this->Disable();
//		DoBlink = false;
//
//		return;
//	}
//
//	const auto& Minecraft = ThreadClasses.Minecraft;
//	const auto& LocalPlayer = ThreadClasses.Player;
//	const auto& World = ThreadClasses.World;
//	const auto& MovingObjectPosition = Minecraft->GetObjectMouseOverPtr(Env);
//
//	if (GetTickCount64() - Timer >= Timeout)
//	{
//		if (TimeoutMode == DISABLE)
//		{
//			this->Disable();
//			DoBlink = false;
//		}
//
//		if (TimeoutMode == AUTOSEND) 
//		{
//			DoBlink = false;
//			Sleep(TimeBeforeAutoSend);
//			DoBlink = true;
//
//			Timer = GetTickCount64();
//		}
//
//		Sleep(1);
//		return;
//	}
//
//	if (DisableOnTargetDamage && MovingObjectPosition->IsAimingEntity(Env) && (GetAsyncKeyState(VK_LBUTTON) & 0x8000))
//	{
//		this->Disable();
//		DoBlink = false;
//
//		return;
//	}
//
//	if (DisableOnLocalDamage && LocalPlayer->GetHurtTime(Env) > 0)
//	{
//		this->Disable();
//		DoBlink = false;
//
//		return;
//	}
//
//	DoBlink = true;
//}
//
//void Blink::OnPacketReceive(JNIEnv* Env, const std::shared_ptr<PacketReceiveData>& PacketData, bool* ShouldCancel)
//{
//	auto ReleasePackets = [Env, PacketData, ShouldCancel, this]() -> void
//	{
//		std::unique_lock<std::mutex> Lock(Mutex);
//
//		if (ReceivedPacketQueue.empty())  {
//			return;
//		}
//
//		ModuleManager::SkipRead0Event = true;
//
//		for (const auto& Packet : ReceivedPacketQueue)
//		{
//			Packet->NetworkManagerInstance->ChannelRead0(Env, Packet->ChannelHandlerContextInstance->GetInstanceObject(), Packet->PacketInstance->GetInstanceObject());
//
//			Env->DeleteGlobalRef(Packet->ChannelHandlerContextInstance->GetInstanceObject());
//			Env->DeleteGlobalRef(Packet->NetworkManagerInstance->GetInstanceObject());
//			Env->DeleteGlobalRef(Packet->PacketInstance->GetInstanceObject());
//
//			Packet->NetworkManagerInstance->UpdateInstanceObject(jobject());
//			Packet->ChannelHandlerContextInstance->UpdateInstanceObject(jobject());
//			Packet->PacketInstance->UpdateInstanceObject(jobject());
//		}
//
//		ReceivedPacketQueue.clear();
//		ModuleManager::SkipRead0Event = false;
//	};
//
//	auto AddPacketToQueue = [Env, PacketData, ShouldCancel, this]() -> void
//	{
//		std::unique_lock<std::mutex> Lock(Mutex);
//		ReceivedPacketQueue.push_back(PacketData);
//	};
//
//	auto IsAttackPacket = [Env, PacketData, ShouldCancel]() -> bool
//	{
//		GameClasses ThreadClasses;
//		if (!ClassRegister::CheckSanityAndGetClasses(Env, &ThreadClasses)) {
//			return false;
//		}
//
//		const auto LocalPlayer = ThreadClasses.Player;
//		const auto Packet = PacketData->PacketInstance;
//		if (!Packet->IsAttackPacket(Env)) {
//			return false;
//		}
//
//		const auto AttackPacket = std::make_shared<::AttackPacket>(Env, Packet->GetInstanceObject());
//		if (AttackPacket->GetEntityID(Env) != LocalPlayer->GetEntityID(Env)) {
//			return false;
//		}
//
//		return AttackPacket->GetLogicOpCode(Env) == static_cast<jbyte>(2);
//	};
//
//	if (DisableOnLocalDamage && IsAttackPacket())
//	{
//		ReleasePackets();
//		this->Disable();
//
//		return;
//	}
//
//	if (!(Direction == BOTH || Direction == RECEIVE))
//	{
//		ReleasePackets();
//		return;
//	}
//
//	if (!DoBlink) 
//	{
//		ReleasePackets();
//		return;
//	}
//
//	AddPacketToQueue();
//	*ShouldCancel = true;
//}
//
//void Blink::OnPacketSend(JNIEnv* Env, const std::shared_ptr<PacketSendData>& PacketData, bool* ShouldCancel)
//{
//	auto ReleasePackets = [Env, PacketData, ShouldCancel, this]() -> void
//	{
//		if (SendPacketQueue.empty()) {
//			return;
//		}
//
//		ModuleManager::SkipAddToSendQueueEvent = true;
//
//		for (const auto& Packet : SendPacketQueue)
//		{
//			PacketData->NetHandlerInstance->AddToSendQueue(Env, Packet->PacketInstance->GetInstanceObject());
//		}
//
//		SendPacketQueue.clear();
//		ModuleManager::SkipAddToSendQueueEvent = false;
//	};
//
//	if (DisableOnTargetDamage && PacketData->PacketInstance->IsUseEntityPacket(Env))
//	{
//		DoBlink = false;
//		ReleasePackets();
//
//		this->Disable();
//
//		return;
//	}
//
//	if (!(Direction == BOTH || Direction == SEND))
//	{
//		ReleasePackets();
//		return;
//	}
//
//	if (!DoBlink)
//	{
//		ReleasePackets();
//		return;
//	}
//
//	SendPacketQueue.push_back(PacketData);
//	*ShouldCancel = true;
//}
//
//void Blink::OnAttackEntity(JNIEnv* Env, jobject TargetInstance, bool* ShouldCancel)
//{
//	if (!DisableOnTargetDamage) {
//		return;
//	}
//
//	DoBlink = false;
//}
