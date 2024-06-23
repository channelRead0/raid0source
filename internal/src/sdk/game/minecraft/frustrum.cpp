#include "pch.h"
#include "frustrum.hpp"

FrustRum::FrustRum(JNIEnv* Env)
{
	this->FrustRumMapper = this->FrustRumMapper ? this->FrustRumMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/culling/Frustum");
	const auto Initializer = Env->GetMethodID(FrustRumMapper->GetClass(), "<init>", "()V");
	this->InstanceObject = Env->NewObject(FrustRumMapper->GetClass(), Initializer);
}

FrustRum::FrustRum(JNIEnv* Env, jobject InstanceObject)
{
	this->FrustRumMapper = this->FrustRumMapper ? this->FrustRumMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/culling/Frustum");
	this->InstanceObject = InstanceObject;
}

void FrustRum::SetPosition(JNIEnv* Env, Vector3<double> Position)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	FrustRumMapper->CallVoidMethod(Env, InstanceObject, "setPosition", Position.X, Position.Y, Position.Z);
}

bool FrustRum::IsBoundingBoxInFrustum(JNIEnv* Env, jobject BoundingBox)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	FrustRumMapper->CallBooleanMethod(Env, InstanceObject, "isBoundingBoxInFrustum", BoundingBox);
}

FrustRum::~FrustRum()
{
}
