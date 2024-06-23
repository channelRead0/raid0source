#pragma once
#include "../../module.hpp"

class RightClicker : public Module
{
private:
	RandomNumberGenerator Random;
public:
	inline static bool Toggled = false;
	inline static bool OnRmb = true;
	inline static bool BlocksOnly = true;

	inline static float TargetCPS = 20.F;

	//todo: whitelist and somne more shit im jus playing bedwars rn shit so fun lmao

	virtual void Run(JNIEnv* Env) override;
};