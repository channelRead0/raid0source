#include "pch.h"
#include "iblockstate.hpp"
#include <sdk/internal/register/envregister.hpp>

IBlockState::IBlockState(JNIEnv* Env)
{
	this->IBlockStateMapper = this->IBlockStateMapper ? this->IBlockStateMapper : std::make_shared<GameClass>(Env, "net/minecraft/block/state/IBlockState");
}

IBlockState::IBlockState(JNIEnv* Env, jobject InstanceObject)
{
	this->IBlockStateMapper = this->IBlockStateMapper ? this->IBlockStateMapper : std::make_shared<GameClass>(Env, "net/minecraft/block/state/IBlockState");
	this->InstanceObject = InstanceObject;
}

std::shared_ptr<Block> IBlockState::GetBlockPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<Block>(Env, jobject());
	}

	if (const auto Block = IBlockStateMapper->CallObjectMethod(Env, InstanceObject, "getBlock")) {
		return std::make_shared<::Block>(Env, Block);
	}

	return std::make_shared<Block>(Env, jobject());
}

IBlockState::~IBlockState()
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
