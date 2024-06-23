#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class CTeam : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> TeamMapper = nullptr;
public:
	CTeam(JNIEnv* Env);
	CTeam(JNIEnv* Env, jobject InstanceObject);


	~CTeam();
};