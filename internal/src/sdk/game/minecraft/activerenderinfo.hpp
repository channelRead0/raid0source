#pragma once

#include <concurrent_vector.h>

#include "player.hpp"
#include "slot.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class ActiveRenderInfo : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> ActiveRenderInfoMapper = nullptr;
	inline static std::shared_ptr<GameClass> FloatBufferMapper = nullptr;
public:
	ActiveRenderInfo(JNIEnv* Env);

	std::vector<float> GetModelView(JNIEnv* Env) const;
	concurrency::concurrent_vector<float> GetModelViewVectorSafe(JNIEnv* Env) const;
	concurrency::concurrent_vector<float> GetProjectionVectorSafe(JNIEnv* Env) const;
	std::vector<float> GetProjection(JNIEnv* Env) const;

	~ActiveRenderInfo();
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class ActiveRenderInfo1_7 : public ActiveRenderInfo
{
public:
};

class ActiveRenderInfo1_8 : public ActiveRenderInfo
{
public:
};