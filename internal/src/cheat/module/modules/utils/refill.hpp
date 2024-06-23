#pragma once
#include "../../module.hpp"

class Refill : public Module
{
private:
	RandomNumberGenerator Random;
public:
	enum MODE
	{
		ON_OPEN_INVENTORY = 0,
		AUTOMATICALLY_OPEN = 1,
	};

	inline static int RefillMode = ON_OPEN_INVENTORY;
	inline static int StartDelay = 50;
	inline static int RefillDelay = 50;

	inline static bool CloseOnSteal = true;
	inline static bool DisableOnSteal = true;
	inline static bool RandomSlots = true;

	virtual void Run(JNIEnv* Env) override;
};