#pragma once
#include "../../module.hpp"
#include <globals/utilities/maths/random/rng.hpp>

class AutoTool : public Module
{
private:
	RandomNumberGenerator Random;
public:

	inline static int StartDelay = 150;
	inline static int SwitchDelay = 50;

	inline static bool SneakingOnly = true;

	virtual void OnTick(JNIEnv* Env) override;
};