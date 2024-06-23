#pragma once
#include "cheat/hook/javahook.hpp"

class LoadWorld
{
public:
	static void LoadWorldDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};
