#pragma once
#include "cheat/hook/javahook.hpp"

class AttackEntity
{
public:
	static void AttackEntityDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};
