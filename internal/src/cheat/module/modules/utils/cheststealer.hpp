#pragma once
#include "../../module.hpp"

class ChestStealer : public Module
{
private:
	RandomNumberGenerator Random;
public:
	inline static int StealDelay = 150;
	inline static int MaxDelay = 150;
	inline static int MinDelay = 150;

	inline static bool CloseOnSteal = true;
	inline static bool DisableOnSteal = true;
	inline static bool RandomSlots = true;

	virtual void OnTick(JNIEnv* Env) override;
};

