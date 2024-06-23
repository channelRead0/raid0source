#pragma once
#include "../../module.hpp"

class HitSelect : public Module
{
private:
	RandomNumberGenerator Random;
	std::atomic<int> ComboCount = 0;
public:

};