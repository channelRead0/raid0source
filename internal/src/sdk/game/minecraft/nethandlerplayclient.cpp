#include "pch.h"
#include "nethandlerplayclient.hpp"

#include "sdk/internal/register/envregister.hpp"

NetHandlerPlayClient::NetHandlerPlayClient(JNIEnv* Env)
{
	this->NetHandlerPlayClientMapper = this->NetHandlerPlayClientMapper ? this->NetHandlerPlayClientMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/network/NetHandlerPlayClient");
}

NetHandlerPlayClient::NetHandlerPlayClient(JNIEnv* Env, jobject InstanceObject)
{
	this->NetHandlerPlayClientMapper = this->NetHandlerPlayClientMapper ? this->NetHandlerPlayClientMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/network/NetHandlerPlayClient");
	this->InstanceObject = InstanceObject;
}

void NetHandlerPlayClient::AddToSendQueue(JNIEnv* Env, jobject Packet) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	NetHandlerPlayClientMapper->CallVoidMethod(Env, InstanceObject, "addToSendQueue", Packet);
}

std::shared_ptr<NetworkManager> NetHandlerPlayClient::GetNetworkManager(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<NetworkManager>(Env, jobject());
	}

	if (const auto NetworkManagerObject = NetHandlerPlayClientMapper->CallObjectMethod(Env, InstanceObject, "getNetworkManager")) {
		return std::make_shared<NetworkManager>(Env, NetworkManagerObject);
	}

	return std::make_shared<NetworkManager>(Env, jobject());
}

NetHandlerPlayClient::~NetHandlerPlayClient()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }
	}

	InstanceObject = nullptr;
}
