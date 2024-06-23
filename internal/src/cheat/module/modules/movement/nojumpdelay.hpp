#pragma once
#include "../../module.hpp"

class NoJumpDelay : public Module
{
private:
public:
	virtual void Run(JNIEnv* Env) override;
};