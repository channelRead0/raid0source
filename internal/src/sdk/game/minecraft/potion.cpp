#include "pch.h"
#include "potion.hpp"

#include "sdk/internal/register/envregister.hpp"

Potion::Potion(JNIEnv* Env)
{
	this->PotionMapper = this->PotionMapper ? this->PotionMapper : std::make_shared<GameClass>(Env, "net/minecraft/potion/Potion");
}

Potion::Potion(JNIEnv* Env, jobject InstanceObject)
{
	this->PotionMapper = this->PotionMapper ? this->PotionMapper : std::make_shared<GameClass>(Env, "net/minecraft/potion/Potion");
	this->InstanceObject = InstanceObject;
}

std::shared_ptr<Potion> Potion::FireResistance(JNIEnv* Env) const
{
	if (const auto FireResistancePotionInstanceObject = PotionMapper->GetObjectField(Env, PotionMapper->GetClass(), "fireResistance")) {
		return std::make_shared<Potion>(Env, FireResistancePotionInstanceObject);
	}

	return std::make_shared<Potion>(Env, jobject());
}

Potion::~Potion()
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
