#pragma once
#include "../../module.hpp"

class BridgeAssist : public Module
{
private:
	RandomNumberGenerator Random;
public:
	inline static bool Toggled = false;
	inline static bool RmbOnly = true;
	inline static bool CheckPitch = true;
	inline static bool BlocksOnly = true;
	inline static bool BackwardOnly = true;

	inline static int Delay = 150;

	virtual void Run(JNIEnv* Env) override;
};