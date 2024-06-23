#pragma once
#include "cheat/hook/javahook.hpp"

class HandleEntityVelocity
{
public:
	static void HandleEntityVelocityDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};
