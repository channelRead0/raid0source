#pragma once
#include "cheat/hook/javahook.hpp"

class SwingItem
{
public:
	static void SwingItemDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};
