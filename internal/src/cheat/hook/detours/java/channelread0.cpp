#include "pch.h"
#include "channelread0.hpp"
#include <cheat/module/module.hpp>

#include "sdk/internal/register/envregister.hpp"
#include "src/sdk/game/minecraft/packet.hpp"
#include "src/cheat/module/module.hpp"

void ChannelRead0::ChannelRead0Detour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
{
	//std::cout << "Hello1" << std::endl;
	if (ModuleManager::SkipRead0())
	{
		return;
	}

	if (INSTANCE_DESTRUCT) 
	{
		//EnvRegister::DestroyThreadEnv();
		return;
	}

	//if (!InstancePtr->GetJniEnv()) {
	//	return;
	//}

	JNIEnv* Env = Thread->GetEnv();

	EnvRegister::RegisterThreadEnv(Env);

	/*static int RunOnce = [&Env, &Frame]()->int
	{
		#ifdef SHOW_CONSOLE
		std::cout << "Detour -> ChannelRead0" << std::endl;
		std::cout << "Env -> 0x" << Env << std::endl;
		std::cout << "This -> 0x" << HookManager->GetObjectParam(Frame, 0) << std::endl;
		#endif
		return 0;
	}();*/

	//NETWORK MANAGER INSTANCE
	if (Env->IsSameObject(HookManager->GetObjectParam(Frame, 0), NULL)) {
		return;
	}

	//CHANNEL HANDLER INSTANCE
	if (Env->IsSameObject(HookManager->GetObjectParam(Frame, 1), NULL)) {
		return;
	}

	//PACKET INSTANCE
	if (Env->IsSameObject(HookManager->GetObjectParam(Frame, 2), NULL)) {
		return;
	}

	//const auto PacketObject = HookManager->GetObjectParam(Frame, 2); //Env->NewLocalRef(HookManager->GetObjectParam(Frame, 2));
	//const auto NetworkManagerObject = HookManager->GetObjectParam(Frame, 0);  //Env->NewLocalRef(HookManager->GetObjectParam(Frame, 0));
	//const auto ChannelHandlerContextObject = HookManager->GetObjectParam(Frame, 1); //Env->NewLocalRef(HookManager->GetObjectParam(Frame, 1));

	const auto PacketInstance = std::make_shared<Packet>(Env, Env->NewLocalRef(HookManager->GetObjectParam(Frame, 2)));
	const auto NetworkManagerInstance = std::make_shared<NetworkManager>(Env, Env->NewLocalRef(HookManager->GetObjectParam(Frame, 0)));
	const auto ChannelHandlerContextInstance = std::make_shared<ChannelHandlerContext>(Env, Env->NewLocalRef(HookManager->GetObjectParam(Frame, 1)));
	const auto ReceivedData = std::make_shared<PacketReceiveData>(NetworkManagerInstance, ChannelHandlerContextInstance, PacketInstance);

	//if (PacketInstance->IsPlayerDiggingPacket(Env))
	//{
	//	//std::cout << "Is Digging" << std::endl;
	//}

	if (PacketInstance->IsRespawnPacket(Env))
	{
		for (const auto& [Name, Mod] : ModuleManager::GetModules())
		{
			Env->PushLocalFrame(100);
			Mod->OnRespawn(Env);
			Env->PopLocalFrame(NULL);
		}

		for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
		{
			Env->PushLocalFrame(100);
			Mod->OnRespawn(Env);
			Env->PopLocalFrame(NULL);
		}
	}

	if (PacketInstance->IsAttackPacket(Env))
	{
		const auto AttackPacket = std::make_shared<::AttackPacket>(Env, PacketInstance->GetInstanceObject());

		for (const auto& [Name, Mod] : ModuleManager::GetModules())
		{
			Env->PushLocalFrame(100);
			Mod->OnPacketLocalEntityAttack(Env, AttackPacket, ShouldCancel);
			Env->PopLocalFrame(NULL);

			if (*ShouldCancel == true) {
				return;
			}
		}

		for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
		{
			Env->PushLocalFrame(100);
			Mod->OnPacketLocalEntityAttack(Env, AttackPacket, ShouldCancel);
			Env->PopLocalFrame(NULL);

			if (*ShouldCancel == true) {
				return;
			}
		}

		AttackPacket->UpdateInstanceObject(jobject());
	}


	for (const auto& [Name, Mod] : ModuleManager::GetModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnPacketReceive(Env, ReceivedData, ShouldCancel);
		Env->PopLocalFrame(NULL);

		if (*ShouldCancel == true) {
			return;
		}
	}

	for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnPacketReceive(Env, ReceivedData, ShouldCancel);
		Env->PopLocalFrame(NULL);

		if (*ShouldCancel == true) {
			//std::cout << "Cancelled" << std::endl;
			return;
		}
	}

	return;
}
