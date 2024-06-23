#include "pch.h"
#include "serverdata.hpp"

#include "sdk/internal/register/envregister.hpp"

ServerData::ServerData(JNIEnv* Env)
{
	this->ServerDataMapper = this->ServerDataMapper ? this->ServerDataMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/multiplayer/ServerData");
}

ServerData::ServerData(JNIEnv* Env, jobject InstanceObject)
{
	this->ServerDataMapper = this->ServerDataMapper ? this->ServerDataMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/multiplayer/ServerData");
	this->InstanceObject = InstanceObject;
}

std::string ServerData::ServerIP(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ServerDataMapper->GetStringField(Env, InstanceObject, "serverIP");
}

std::string ServerData::ServerName(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return ServerDataMapper->GetStringField(Env, InstanceObject, "serverName");
}

ServerData::~ServerData()
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

