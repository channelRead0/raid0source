#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class RenderManager : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> RenderManagerMapper = nullptr;
public:
	RenderManager(JNIEnv* Env);
	RenderManager(JNIEnv* Env, jobject InstanceObject);

	Vector3<double> GetRenderPos(JNIEnv* Env) const;
	Vector2<double> GetPlayerView(JNIEnv* Env) const;

	bool RenderEntitySimple(JNIEnv* Env, jobject Entity, float PartialTicks) const;

	~RenderManager();
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class RenderManager1_7 : public RenderManager
{
public:
};

class RenderManager1_8 : public RenderManager
{
public:
};