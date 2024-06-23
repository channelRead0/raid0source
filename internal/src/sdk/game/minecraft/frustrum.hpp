#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class FrustRum : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> FrustRumMapper = nullptr;
	//net/minecraft/client/gui/inventory/GuiInventory
public:
	FrustRum(JNIEnv* Env);
	FrustRum(JNIEnv* Env, jobject InstanceObject);

	void SetPosition(JNIEnv* Env, Vector3<double> Position);
	bool IsBoundingBoxInFrustum(JNIEnv* Env, jobject BoundingBox);

	~FrustRum();
};