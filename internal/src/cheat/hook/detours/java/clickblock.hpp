#pragma once
#include "cheat/hook/javahook.hpp"

class ClickBlock
{
public:
	static void ClickBlockDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};
