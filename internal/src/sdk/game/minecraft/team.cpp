#include "pch.h"
#include "team.hpp"
#include <sdk/internal/register/envregister.hpp>

CTeam::CTeam(JNIEnv* Env)
{
	this->TeamMapper = this->TeamMapper ? this->TeamMapper : std::make_shared<GameClass>(Env, "net/minecraft/scoreboard/Team");
}

CTeam::CTeam(JNIEnv* Env, jobject InstanceObject)
{
	this->TeamMapper = this->TeamMapper ? this->TeamMapper : std::make_shared<GameClass>(Env, "net/minecraft/scoreboard/Team");
	this->InstanceObject = InstanceObject;
}

CTeam::~CTeam()
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
