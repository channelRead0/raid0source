#include "pch.h"
#include "movingobjectposition.hpp"

MovingObjectPosition::MovingObjectPosition(JNIEnv* Env)
{
	this->MovingObjectPositionMapper = this->MovingObjectPositionMapper ? this->MovingObjectPositionMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/MovingObjectPosition");
}

MovingObjectPosition::MovingObjectPosition(JNIEnv* Env, jobject InstanceObject)
{
	this->MovingObjectPositionMapper = this->MovingObjectPositionMapper ? this->MovingObjectPositionMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/MovingObjectPosition");
	this->InstanceObject = InstanceObject;
}

bool MovingObjectPosition::IsAimingEntity(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const auto TypeOfHitObject = MovingObjectPositionMapper->GetObjectField(Env, InstanceObject, "typeOfHit");

	if (Env->IsSameObject(InstanceObject, TypeOfHitObject)) {
		return{};
	}

	const auto MovingObjectType = std::make_shared<GameClass>(Env, "net/minecraft/util/MovingObjectPosition$MovingObjectType");
	const auto EntityMovingObject = MovingObjectType->GetObjectField(Env, InstanceObject, "ENTITY");

	if (Env->IsSameObject(InstanceObject, EntityMovingObject)) {
		return{};
	}

	return Env->IsSameObject(EntityMovingObject, TypeOfHitObject);
}

bool MovingObjectPosition::IsAimingBlock(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const auto TypeOfHitObject = MovingObjectPositionMapper->GetObjectField(Env, InstanceObject, "typeOfHit");

	if (Env->IsSameObject(InstanceObject, TypeOfHitObject)) {
		return{};
	}

	const auto MovingObjectType = std::make_shared<GameClass>(Env, "net/minecraft/util/MovingObjectPosition$MovingObjectType");
	const auto EntityMovingObject = MovingObjectType->GetObjectField(Env, InstanceObject, "BLOCK");

	if (Env->IsSameObject(InstanceObject, EntityMovingObject)) {
		return{};
	}

	return Env->IsSameObject(EntityMovingObject, TypeOfHitObject);
}

bool MovingObjectPosition::IsMiss(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	const auto TypeOfHitObject = MovingObjectPositionMapper->GetObjectField(Env, InstanceObject, "typeOfHit");

	if (Env->IsSameObject(InstanceObject, TypeOfHitObject)) {
		return{};
	}

	const auto MovingObjectType = std::make_shared<GameClass>(Env, "net/minecraft/util/MovingObjectPosition$MovingObjectType");
	const auto EntityMovingObject = MovingObjectType->GetObjectField(Env, InstanceObject, "MISS");

	if (Env->IsSameObject(InstanceObject, EntityMovingObject)) {
		return{};
	}

	return Env->IsSameObject(EntityMovingObject, TypeOfHitObject);
}

jobject MovingObjectPosition::BlockPos(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return jobject();
	}

	return MovingObjectPositionMapper->CallObjectMethod(Env, InstanceObject, "getBlockPos");
}

jobject MovingObjectPosition::EntityHit(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return jobject();
	}

	return MovingObjectPositionMapper->GetObjectField(Env, InstanceObject, "entityHit");
}

MovingObjectPosition::~MovingObjectPosition()
{
}
