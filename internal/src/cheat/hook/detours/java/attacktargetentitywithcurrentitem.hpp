#pragma once
#include "cheat/hook/javahook.hpp"

class AttackTargetEntityWithCurrentItem
{
public:
	static void AttackTargetEntityWithCurrentItemDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};
