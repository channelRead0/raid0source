#include "pch.h"
#include "modelrenderer.hpp"

ModelRenderer::ModelRenderer(JNIEnv* Env)
{
	this->ModelRendererMapper = this->ModelRendererMapper ? this->ModelRendererMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/model/ModelRenderer");
}

ModelRenderer::ModelRenderer(JNIEnv* Env, jobject InstanceObject)
{
	this->ModelRendererMapper = this->ModelRendererMapper ? this->ModelRendererMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/model/ModelRenderer");
	this->InstanceObject = InstanceObject;
}

Vector3<float> ModelRenderer::RotationAngle(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return
	{
		ModelRendererMapper->GetFloatField(Env, InstanceObject, "rotateAngleX"),
		ModelRendererMapper->GetFloatField(Env, InstanceObject, "rotateAngleY"),
		ModelRendererMapper->GetFloatField(Env, InstanceObject, "rotateAngleZ"),
	};
}

void ModelRenderer::SetRotationAngle(JNIEnv* Env, Vector3<float> New) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	ModelRendererMapper->SetFloatField(Env, InstanceObject, "rotateAngleX", New.X);
	ModelRendererMapper->SetFloatField(Env, InstanceObject, "rotateAngleY", New.Z);
	ModelRendererMapper->SetFloatField(Env, InstanceObject, "rotateAngleZ", New.Y);
}

Vector3<float> ModelRenderer::RotationPoint(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return
	{
		ModelRendererMapper->GetFloatField(Env, InstanceObject, "rotationPointX"),
		ModelRendererMapper->GetFloatField(Env, InstanceObject, "rotationPointY"),
		ModelRendererMapper->GetFloatField(Env, InstanceObject, "rotationPointZ"),
	};
}

void ModelRenderer::SetRotationPoint(JNIEnv* Env, Vector3<float> New) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	ModelRendererMapper->SetFloatField(Env, InstanceObject, "rotationPointX", New.X);
	ModelRendererMapper->SetFloatField(Env, InstanceObject, "rotationPointY", New.Z);
	ModelRendererMapper->SetFloatField(Env, InstanceObject, "rotationPointZ", New.Y);
}

ModelRenderer::~ModelRenderer()
{
}
