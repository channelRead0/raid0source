#pragma once
#include "cheat/module/module.hpp"
#include <concurrent_vector.h>

class Friends : public Module
{
public:
	virtual void Run(JNIEnv* Env) override;
	static bool IsFriend(int EntityID);
};