#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class OpenGlHelper : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> OpenGlHelperMapper = nullptr;
	//net/minecraft/client/gui/inventory/GuiInventory
public:
	OpenGlHelper(JNIEnv* Env);
	OpenGlHelper(JNIEnv* Env, jobject InstanceObject);

	void GlBlendFunc(JNIEnv* Env, int sFactorRGB, int dFactorRGB, int sfactorAlpha, int dfactorAlpha);
	void GlAttachShader(JNIEnv* Env, int Program, int Shader);
	void GlLinkProgram(JNIEnv* Env, int Program);
	void GlUseProgram(JNIEnv* Env, int Program);
	void SetActiveTexture(JNIEnv* Env, int Texture);
	bool ArbShaders(JNIEnv* Env);

	int LightmapTexUnit(JNIEnv* Env);

	int DefaultTexUnit(JNIEnv* Env);

	~OpenGlHelper();
};