#include "pch.h"
#include "glstatemanager.hpp"

GlStateManager::GlStateManager(JNIEnv* Env)
{
	this->GlStateManagerMapper = this->GlStateManagerMapper ? this->GlStateManagerMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/GlStateManager");
	this->InstanceObject = static_cast<jobject>(GlStateManagerMapper->GetClass());
}

GlStateManager::GlStateManager(JNIEnv* Env, jobject InstanceObject)
{
	this->GlStateManagerMapper = this->GlStateManagerMapper ? this->GlStateManagerMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/GlStateManager");
	this->InstanceObject = InstanceObject;
}

void GlStateManager::SetDepth(JNIEnv* Env, bool Enabled) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	GlStateManagerMapper->CallVoidMethod(Env, InstanceObject, Enabled ? "enableDepth" : "disableDepth");
}

void GlStateManager::PushMatrix(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	GlStateManagerMapper->CallVoidMethod(Env, InstanceObject, "pushMatrix");
}

void GlStateManager::PopMatrix(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	GlStateManagerMapper->CallVoidMethod(Env, InstanceObject, "popMatrix");
}

void GlStateManager::PushAttrb(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	GlStateManagerMapper->CallVoidMethod(Env, InstanceObject, "pushAttrib");
}

void GlStateManager::PopAttrib(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	GlStateManagerMapper->CallVoidMethod(Env, InstanceObject, "popAttrib");
}

GlStateManager::~GlStateManager()
{
}
