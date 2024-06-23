#include "pch.h"
#include "fontrenderer.hpp"

#include "sdk/internal/register/envregister.hpp"

FontRenderer::FontRenderer(JNIEnv* Env)
{
	this->FontRendererMapper = this->FontRendererMapper ? this->FontRendererMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/gui/FontRenderer");
}

FontRenderer::FontRenderer(JNIEnv* Env, jobject InstanceObject)
{
	this->FontRendererMapper = this->FontRendererMapper ? this->FontRendererMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/gui/FontRenderer");
	this->InstanceObject = InstanceObject;
}

FontRenderer::~FontRenderer()
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
