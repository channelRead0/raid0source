#include "pch.h"
#include "worldinfo.hpp"

#include "sdk/internal/register/envregister.hpp"

WorldInfo::WorldInfo(JNIEnv* Env)
{
	this->WorldInfoMapper = this->WorldInfoMapper ? this->WorldInfoMapper : std::make_shared<GameClass>(Env, "net/minecraft/world/storage/WorldInfo");
}

WorldInfo::WorldInfo(JNIEnv* Env, jobject InstanceObject)
{
	this->WorldInfoMapper = this->WorldInfoMapper ? this->WorldInfoMapper : std::make_shared<GameClass>(Env, "net/minecraft/world/storage/WorldInfo");
	this->InstanceObject = InstanceObject;
}

std::string WorldInfo::GetWorldName(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return WorldInfoMapper->CallStringMethod(Env, InstanceObject, "getWorldName");
}

WorldInfo::~WorldInfo()
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
