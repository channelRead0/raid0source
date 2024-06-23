#include "pch.h"
#include "blockpos.hpp"

#include "sdk/internal/register/envregister.hpp"

BlockPos::BlockPos(JNIEnv* Env)
{
	this->BlockPosMapper = this->BlockPosMapper ? this->BlockPosMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/BlockPos");
}

BlockPos::BlockPos(JNIEnv* Env, jobject InstanceObject)
{
	this->BlockPosMapper = this->BlockPosMapper ? this->BlockPosMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/BlockPos");
	this->InstanceObject = InstanceObject;
}

jobject BlockPos::NewObject(JNIEnv* Env, const Vector3<double>& Position)
{
	const auto InitializerMethodID = Env->GetMethodID(BlockPosMapper->GetClass(), "<init>", "(DDD)V");

	if (!InitializerMethodID) {
		return{};
	}

	auto NewObjectInstance = Env->NewObject(BlockPosMapper->GetClass(), InitializerMethodID, Position.X, Position.Y, Position.Z);

	if (!NewObjectInstance)
	{
		return{};
	}

	NewObjectInstance = Env->NewLocalRef(NewObjectInstance);

	this->InstanceObject = NewObjectInstance;

	return NewObjectInstance;
}

BlockPos::~BlockPos()
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
