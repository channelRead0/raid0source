#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class BlockPos : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> BlockPosMapper = nullptr;
public:
	BlockPos(JNIEnv* Env);
	BlockPos(JNIEnv* Env, jobject InstanceObject);

	jobject NewObject(JNIEnv* Env, const Vector3<double>& Position);

	~BlockPos();
};
