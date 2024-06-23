#pragma once
#include "../../module.hpp"

class Sprint : public Module
{
private:
public:
	inline static bool DisableOnInvisible = true;

	virtual void Run(JNIEnv* Env) override;
};