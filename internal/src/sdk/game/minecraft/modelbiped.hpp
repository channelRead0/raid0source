#pragma once

#include "modelrenderer.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

struct BipedModel
{
	/*public ModelRenderer bipedHead;
	public ModelRenderer bipedBody;
	public ModelRenderer bipedRightArm;
	public ModelRenderer bipedLeftArm;
	public ModelRenderer bipedRightLeg;
	public ModelRenderer bipedLeftLeg;*/
	std::shared_ptr<ModelRenderer> BipedHead;
	std::shared_ptr<ModelRenderer> BipedBody;
	std::shared_ptr<ModelRenderer> BipedRightArm;
	std::shared_ptr<ModelRenderer> BipedLeftArm;
	std::shared_ptr<ModelRenderer> BipedRightLeg;
	std::shared_ptr<ModelRenderer> BipedLeftLeg;

};

class ModelBiped : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> ModelBipedMapper = nullptr;
public:
	ModelBiped(JNIEnv* Env);
	ModelBiped(JNIEnv* Env, jobject InstanceObject);

	BipedModel GetModel(JNIEnv* Env) const;

	~ModelBiped();
};