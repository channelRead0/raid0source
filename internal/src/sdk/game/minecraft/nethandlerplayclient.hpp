#pragma once

#include "networkmanager.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class NetHandlerPlayClient : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> NetHandlerPlayClientMapper = nullptr;
public:
	NetHandlerPlayClient(JNIEnv* Env);
	NetHandlerPlayClient(JNIEnv* Env, jobject InstanceObject);

	void AddToSendQueue(JNIEnv* Env, jobject Packet) const;
	std::shared_ptr<NetworkManager> GetNetworkManager(JNIEnv* Env) const;

	~NetHandlerPlayClient();
};
