#pragma once

#include "minecraft.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class MovingObjectPosition : public ClassBase {
private:
	inline static std::shared_ptr<GameClass> MovingObjectPositionMapper = nullptr;
public:
	MovingObjectPosition(JNIEnv* Env);
	MovingObjectPosition(JNIEnv* Env, jobject InstanceObject);

	bool IsAimingEntity(JNIEnv* Env) const;
	bool IsAimingBlock(JNIEnv* Env) const;
	bool IsMiss(JNIEnv* Env) const;
	jobject BlockPos(JNIEnv* Env) const;
	jobject EntityHit(JNIEnv* Env) const;

	~MovingObjectPosition();
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here