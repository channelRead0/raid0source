#include "pch.h"
#include "block.hpp"

#include "sdk/internal/register/envregister.hpp"

Block::Block(JNIEnv* Env)
{
	this->BlockAirMapper = this->BlockAirMapper ? this->BlockAirMapper : std::make_shared<GameClass>(Env, "net/minecraft/block/BlockAir");
	this->BlockMapper = this->BlockMapper ? this->BlockMapper : std::make_shared<GameClass>(Env, "net/minecraft/block/Block");
}

Block::Block(JNIEnv* Env, jobject InstanceObject)
{
	this->BlockAirMapper = this->BlockAirMapper ? this->BlockAirMapper : std::make_shared<GameClass>(Env, "net/minecraft/block/BlockAir");
	this->BlockMapper = this->BlockMapper ? this->BlockMapper : std::make_shared<GameClass>(Env, "net/minecraft/block/Block");
	this->InstanceObject = InstanceObject;
}

bool Block::IsAir(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return Env->IsInstanceOf(InstanceObject, BlockAirMapper->GetClass());
}

std::string Block::GetUnlocalizedName(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return BlockMapper->CallStringMethod(Env, InstanceObject, "getUnlocalizedName");
}

Block::~Block()
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
