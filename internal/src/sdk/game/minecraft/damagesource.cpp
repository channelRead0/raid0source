#include "pch.h"
#include "damagesource.hpp"

#include "sdk/internal/register/envregister.hpp"

DamageSource::DamageSource(JNIEnv* Env)
{
	this->DamageSourceMapper = this->DamageSourceMapper ? this->DamageSourceMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/DamageSource");
}

DamageSource::DamageSource(JNIEnv* Env, jobject InstanceObject)
{
	this->DamageSourceMapper = this->DamageSourceMapper ? this->DamageSourceMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/DamageSource");
	this->InstanceObject = InstanceObject;
}

std::shared_ptr<DamageSource> DamageSource::OnFireDamageSource(JNIEnv* Env) const
{
	if (const auto OnFireInstanceObject = DamageSourceMapper->GetObjectField(Env, DamageSourceMapper->GetClass(), "onFire")) {
		return std::make_shared<DamageSource>(Env, OnFireInstanceObject);
	}

	return std::make_shared<DamageSource>(Env, jobject());
}

DamageSource::~DamageSource()
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
