#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class EntityRenderer : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> EntityRendererMapper = nullptr;
public:
	EntityRenderer(JNIEnv* Env);
	EntityRenderer(JNIEnv* Env, jobject InstanceObject);

	jobject GetPointedEntity(JNIEnv* Env) const;
	std::shared_ptr<Player> GetPointedEntityPtr(JNIEnv* Env) const;
	void SetupCameraTransformation(JNIEnv* Env, float RenderPartialTicks, int Pass) const;
	void SetupOverlayRendering(JNIEnv* Env) const;

	void SetCameraYaw(JNIEnv* Env, double Yaw) const;
	void SetCameraPitch(JNIEnv* Env, double Pitch) const;

	~EntityRenderer();
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class EntityRenderer1_7 : public EntityRenderer
{
public:
};

class EntityRenderer1_8 : public EntityRenderer
{
public:
};