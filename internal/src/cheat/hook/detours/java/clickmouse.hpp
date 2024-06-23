#pragma once
#include "cheat/hook/javahook.hpp"

class ClickMouse
{
public:
	static void ClickMouseDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};
