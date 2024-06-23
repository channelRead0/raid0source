#include "pch.h"
#include "rendermanager.hpp"

#include "sdk/internal/register/envregister.hpp"

RenderManager::RenderManager(JNIEnv* Env)
{
	this->RenderManagerMapper = this->RenderManagerMapper ? this->RenderManagerMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/entity/RenderManager");
}

RenderManager::RenderManager(JNIEnv* Env, jobject InstanceObject)
{
	this->RenderManagerMapper = this->RenderManagerMapper ? this->RenderManagerMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/entity/RenderManager");
	this->InstanceObject = InstanceObject;
}

Vector3<double> RenderManager::GetRenderPos(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return
	{
		RenderManagerMapper->GetDoubleField(Env, InstanceObject, "renderPosX"),
		RenderManagerMapper->GetDoubleField(Env, InstanceObject, "renderPosY"),
		RenderManagerMapper->GetDoubleField(Env, InstanceObject, "renderPosZ")
	};
}

Vector2<double> RenderManager::GetPlayerView(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return
	{
		RenderManagerMapper->GetFloatField(Env, InstanceObject, "playerViewX"),
		RenderManagerMapper->GetFloatField(Env, InstanceObject, "playerViewY"),
	};
}

bool RenderManager::RenderEntitySimple(JNIEnv* Env, jobject Entity, float PartialTicks) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return RenderManagerMapper->CallBooleanMethod(Env, InstanceObject, "renderEntitySimple", Entity, PartialTicks);
}

RenderManager::~RenderManager()
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
