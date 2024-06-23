#include "pch.h"
#include "openglhelper.hpp"

OpenGlHelper::OpenGlHelper(JNIEnv* Env)
{
	this->OpenGlHelperMapper = this->OpenGlHelperMapper ? this->OpenGlHelperMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/OpenGlHelper");
	this->InstanceObject = static_cast<jobject>(OpenGlHelperMapper->GetClass());
}

OpenGlHelper::OpenGlHelper(JNIEnv* Env, jobject InstanceObject)
{
	this->OpenGlHelperMapper = this->OpenGlHelperMapper ? this->OpenGlHelperMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/OpenGlHelper");
	this->InstanceObject = InstanceObject;
}

void OpenGlHelper::GlBlendFunc(JNIEnv* Env, int sFactorRGB, int dFactorRGB, int sfactorAlpha, int dfactorAlpha)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	OpenGlHelperMapper->CallVoidMethod(Env, InstanceObject, "glBlendFunc", sFactorRGB, dFactorRGB, sfactorAlpha, dfactorAlpha);
}

void OpenGlHelper::GlAttachShader(JNIEnv* Env, int Program, int Shader)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}
	
	OpenGlHelperMapper->CallVoidMethod(Env, InstanceObject, "glAttachShader", Program, Shader);
}

void OpenGlHelper::GlLinkProgram(JNIEnv* Env, int Program)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}
	
	OpenGlHelperMapper->CallVoidMethod(Env, InstanceObject, "glLinkProgram", Program);
}

void OpenGlHelper::GlUseProgram(JNIEnv* Env, int Program)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	OpenGlHelperMapper->CallVoidMethod(Env, InstanceObject, "glUseProgram");
}

void OpenGlHelper::SetActiveTexture(JNIEnv* Env, int Texture)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	OpenGlHelperMapper->CallVoidMethod(Env, InstanceObject, "setActiveTexture");
}

bool OpenGlHelper::ArbShaders(JNIEnv* Env)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	OpenGlHelperMapper->GetBooleanField(Env, InstanceObject, "arbShaders"); 
}

int OpenGlHelper::LightmapTexUnit(JNIEnv* Env)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}
	
	return OpenGlHelperMapper->GetIntField(Env, InstanceObject, "lightmapTexUnit");
}

int OpenGlHelper::DefaultTexUnit(JNIEnv* Env)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return OpenGlHelperMapper->GetIntField(Env, InstanceObject, "defaultTexUnit");
}

OpenGlHelper::~OpenGlHelper()
{
}
