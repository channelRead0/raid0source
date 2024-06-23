#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class ScaledResolution : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> ScaledResolutionMapper = nullptr;
	//net/minecraft/client/gui/inventory/GuiInventory
public:
	ScaledResolution(JNIEnv* Env);
	ScaledResolution(JNIEnv* Env, jobject InstanceObject);
	ScaledResolution(JNIEnv* Env, jobject InstanceObject, bool Minecraft = true);

	int GetScaledHeight(JNIEnv* Env);
	int GetScaledWidth(JNIEnv* Env);

	~ScaledResolution();
};