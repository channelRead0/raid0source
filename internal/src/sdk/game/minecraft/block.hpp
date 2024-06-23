#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class Block : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> BlockMapper = nullptr;
	inline static std::shared_ptr<GameClass> BlockAirMapper = nullptr;
public:
	Block(JNIEnv* Env);
	Block(JNIEnv* Env, jobject InstanceObject);

	bool IsAir(JNIEnv* Env) const;
	std::string GetUnlocalizedName(JNIEnv* Env) const;

	~Block();
};