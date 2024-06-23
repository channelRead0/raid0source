#include "pch.h"
#include "modelbiped.hpp"

ModelBiped::ModelBiped(JNIEnv* Env)
{
	this->ModelBipedMapper = this->ModelBipedMapper ? this->ModelBipedMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/model/ModelBiped");
}

ModelBiped::ModelBiped(JNIEnv* Env, jobject InstanceObject)
{
	this->ModelBipedMapper = this->ModelBipedMapper ? this->ModelBipedMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/model/ModelBiped");
	this->InstanceObject = InstanceObject;
}

BipedModel ModelBiped::GetModel(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		{};
	}

	return BipedModel
	(
		std::make_shared<ModelRenderer>(Env, ModelBipedMapper->GetObjectField(Env, InstanceObject, "bipedHead")),
		std::make_shared<ModelRenderer>(Env, ModelBipedMapper->GetObjectField(Env, InstanceObject, "bipedBody")),
		std::make_shared<ModelRenderer>(Env, ModelBipedMapper->GetObjectField(Env, InstanceObject, "bipedRightArm")),
		std::make_shared<ModelRenderer>(Env, ModelBipedMapper->GetObjectField(Env, InstanceObject, "bipedLeftArm")),
		std::make_shared<ModelRenderer>(Env, ModelBipedMapper->GetObjectField(Env, InstanceObject, "bipedRightLeg")),
		std::make_shared<ModelRenderer>(Env, ModelBipedMapper->GetObjectField(Env, InstanceObject, "bipedLeftLeg"))
	);
}

ModelBiped::~ModelBiped()
{
}
