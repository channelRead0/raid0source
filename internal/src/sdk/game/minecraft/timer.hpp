#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class Timer : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> TimerMapper = nullptr;
public:
	Timer(JNIEnv* Env);
	Timer(JNIEnv* Env, jobject InstanceObject);

	float GetRenderPartialTicks(JNIEnv* Env) const;

	~Timer();
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class Timer1_7 : public Timer
{
public:
};

class Timer1_8 : public Timer
{
public:
};