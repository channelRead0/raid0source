#pragma once
#include "cheat/hook/javahook.hpp"

class RenderWorldPass
{
public:
	static void RenderWorldPassDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};
