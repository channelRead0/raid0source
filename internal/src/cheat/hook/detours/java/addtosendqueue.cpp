#include "pch.h"
#include "addtosendqueue.hpp"
#include <cheat/module/module.hpp>

#include "sdk/internal/register/envregister.hpp"

void AddToSendQueue::AddToSendQueueDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel)
{
	if (ModuleManager::SkipAddToSendQueue()) {
		return;
	}

	if (INSTANCE_DESTRUCT)
	{
		//EnvRegister::DestroyThreadEnv();
		return;
	}

	if (!InstancePtr->GetJniEnv()) {
		return;
	}

	JNIEnv* Env = Thread->GetEnv();

	EnvRegister::RegisterThreadEnv(Env);

	/*static int RunOnce = [&Env, &Frame]()->int
	{
		#ifdef SHOW_CONSOLE
		std::cout << "Detour -> AddToSendQueue" << std::endl;
		std::cout << "Env -> 0x" << Env << std::endl;
		std::cout << "This -> 0x" << HookManager->GetObjectParam(Frame, 0) << std::endl;
		std::cout << "Packet -> 0x" << HookManager->GetObjectParam(Frame, 1) << std::endl;
		#endif
		return 0;
	}();*/

	//PACKET INSTANCE
	if (Env->IsSameObject(HookManager->GetObjectParam(Frame, 1), NULL)) {
		return;
	}

	//NHP INSTANCE
	if (Env->IsSameObject(HookManager->GetObjectParam(Frame, 0), NULL)) {
		return;
	}

	//std::cout << GetJavaClassName(Env, Env->GetObjectClass(HookManager->GetObjectParam(Frame, 1))) << std::endl;

	const auto PacketObject = Env->NewGlobalRef(HookManager->GetObjectParam(Frame, 1));
	const auto NetHandlerPlayClientObject = Env->NewGlobalRef(HookManager->GetObjectParam(Frame, 0));
	//const auto PacketObject = Env->NewLocalRef(HookManager->GetObjectParam(Frame, 1));
	const auto PacketInstance = std::make_shared<Packet>(Env, PacketObject);
	const auto NetHandlerPlayClientInstance = std::make_shared<NetHandlerPlayClient>(Env, NetHandlerPlayClientObject);

	const auto PacketSendData = std::make_shared<::PacketSendData>(NetHandlerPlayClientInstance, PacketInstance);

	if (PacketInstance->IsUseEntityPacket(Env))
	{
		const auto PacketUseEntityInstance = std::make_shared<PacketUseEntity>(Env, PacketInstance->GetInstanceObject());

		for (const auto& [Name, Mod] : ModuleManager::GetModules())
		{
			Env->PushLocalFrame(100);
			Mod->OnPacketUseEntity(Env, PacketUseEntityInstance, ShouldCancel);
			Env->PopLocalFrame(NULL);

			if (*ShouldCancel == true) {
				return;
			}
		}

		for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
		{
			Env->PushLocalFrame(100);
			Mod->OnPacketUseEntity(Env, PacketUseEntityInstance, ShouldCancel);
			Env->PopLocalFrame(NULL);

			if (*ShouldCancel == true) {
				return;
			}
		}

		PacketUseEntityInstance->UpdateInstanceObject(jobject());
	}

	for (const auto& [Name, Mod] : ModuleManager::GetModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnPacketSend(Env, PacketSendData, ShouldCancel);
		Env->PopLocalFrame(NULL);

		if (*ShouldCancel == true) {
			return;
		}
	}

	for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
	{
		Env->PushLocalFrame(100);
		Mod->OnPacketSend(Env, PacketSendData, ShouldCancel);
		Env->PopLocalFrame(NULL);

		if (*ShouldCancel == true) {
			return;
		}
	}

	return;
}
