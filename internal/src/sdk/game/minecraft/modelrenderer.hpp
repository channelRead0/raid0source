#pragma once

#include "src/sdk/internal/templates/classbase.hpp"

class ModelRenderer : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> ModelRendererMapper = nullptr;
public:
	ModelRenderer(JNIEnv* Env);
	ModelRenderer(JNIEnv* Env, jobject InstanceObject);

	Vector3<float> RotationAngle(JNIEnv* Env) const;
	void SetRotationAngle(JNIEnv* Env, Vector3<float> New) const;

	Vector3<float> RotationPoint(JNIEnv* Env) const;
	void SetRotationPoint(JNIEnv* Env, Vector3<float> New) const;

	~ModelRenderer();
};