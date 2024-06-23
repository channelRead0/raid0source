#include "pch.h"
#include "axisalignedboundingbox.hpp"

#include "globals/structures/axisalignedbb_t.hpp"
#include "sdk/internal/register/envregister.hpp"

AxisAlignedBoundingBox::AxisAlignedBoundingBox(JNIEnv* Env)
{
	this->AxisAlignedBoundingBoxMapper = this->AxisAlignedBoundingBoxMapper ? this->AxisAlignedBoundingBoxMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/AxisAlignedBB");
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(JNIEnv* Env, jobject InstanceObject)
{
	this->AxisAlignedBoundingBoxMapper = this->AxisAlignedBoundingBoxMapper ? this->AxisAlignedBoundingBoxMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/AxisAlignedBB");
	this->InstanceObject = InstanceObject;
}

jobject AxisAlignedBoundingBox::NewObject(JNIEnv* Env, const AxisAlignedBB_T<double>& Bounds) 
{
	const auto InitializerMethodID = AxisAlignedBoundingBoxMapper->GetMethodID("<init>");

	if (!InitializerMethodID) {
		return{};
	}

	auto NewObjectInstance = Env->NewObject(AxisAlignedBoundingBoxMapper->GetClass(), InitializerMethodID, Bounds.GetMinX(), Bounds.GetMinY(), Bounds.GetMinZ(), Bounds.GetMaxX(), Bounds.GetMaxY(), Bounds.GetMaxZ());

	if (!NewObjectInstance)
	{
		return{};
	}

	NewObjectInstance = Env->NewLocalRef(NewObjectInstance);

	this->InstanceObject = NewObjectInstance;

	return NewObjectInstance;
}

AxisAlignedBB_T<double> AxisAlignedBoundingBox::GetAxisAlignedBoundingBoxNative(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return AxisAlignedBB_T{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	}

	return AxisAlignedBB_T
	{
		AxisAlignedBoundingBoxMapper->GetDoubleField(Env, InstanceObject, "minX"),
		AxisAlignedBoundingBoxMapper->GetDoubleField(Env, InstanceObject, "minY"),
		AxisAlignedBoundingBoxMapper->GetDoubleField(Env, InstanceObject, "minZ"),
		AxisAlignedBoundingBoxMapper->GetDoubleField(Env, InstanceObject, "maxX"),
		AxisAlignedBoundingBoxMapper->GetDoubleField(Env, InstanceObject, "maxY"),
		AxisAlignedBoundingBoxMapper->GetDoubleField(Env, InstanceObject, "maxZ"),
	};
}

AxisAlignedBoundingBox::~AxisAlignedBoundingBox()
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
