#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class GlStateManager : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> GlStateManagerMapper = nullptr;
	//net/minecraft/client/gui/inventory/GuiInventory
public:
	GlStateManager(JNIEnv* Env);
	GlStateManager(JNIEnv* Env, jobject InstanceObject);

	void SetDepth(JNIEnv* Env, bool Enabled) const;
	void PushMatrix(JNIEnv* Env) const;
	void PopMatrix(JNIEnv* Env) const;
	void PushAttrb(JNIEnv* Env) const;
	void PopAttrib(JNIEnv* Env) const;

	~GlStateManager();
};