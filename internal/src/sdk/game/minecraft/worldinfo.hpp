#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class WorldInfo : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> WorldInfoMapper = nullptr;
public:
	WorldInfo(JNIEnv* Env);
	WorldInfo(JNIEnv* Env, jobject InstanceObject);

	std::string GetWorldName(JNIEnv* Env) const;

	~WorldInfo();
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class WorldInfo1_7 : public WorldInfo
{
public:
};

class WorldInfo1_8 : public WorldInfo
{
public:
};