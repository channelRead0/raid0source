#include "pch.h"
#include "entityrenderer.hpp"

#include "sdk/internal/register/envregister.hpp"

EntityRenderer::EntityRenderer(JNIEnv* Env)
{
	this->EntityRendererMapper = this->EntityRendererMapper ? this->EntityRendererMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/EntityRenderer");
}

EntityRenderer::EntityRenderer(JNIEnv* Env, jobject InstanceObject)
{
	this->EntityRendererMapper = this->EntityRendererMapper ? this->EntityRendererMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/EntityRenderer");
	this->InstanceObject = InstanceObject;
}

jobject EntityRenderer::GetPointedEntity(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return EntityRendererMapper->GetObjectField(Env, InstanceObject, "pointedEntity");
}

std::shared_ptr<Player> EntityRenderer::GetPointedEntityPtr(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	if (const auto PointedEntity = EntityRendererMapper->GetObjectField(Env, InstanceObject, "pointedEntity")) {
		return std::make_shared<Player>(Env, PointedEntity);
	}

	return std::make_shared<Player>(Env, jobject());
}

void EntityRenderer::SetupCameraTransformation(JNIEnv* Env, float RenderPartialTicks, int Pass) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityRendererMapper->CallVoidMethod(Env, InstanceObject, "setupCameraTransform", RenderPartialTicks, Pass);
}

void EntityRenderer::SetupOverlayRendering(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}
	
	EntityRendererMapper->CallVoidMethod(Env, InstanceObject, "setupOverlayRendering");
}

void EntityRenderer::SetCameraYaw(JNIEnv* Env, double Yaw) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityRendererMapper->SetDoubleField(Env, InstanceObject, "cameraYaw", Yaw);
}

void EntityRenderer::SetCameraPitch(JNIEnv* Env, double Pitch) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	EntityRendererMapper->SetDoubleField(Env, InstanceObject, "cameraPitch", Pitch);
}

EntityRenderer::~EntityRenderer()
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
