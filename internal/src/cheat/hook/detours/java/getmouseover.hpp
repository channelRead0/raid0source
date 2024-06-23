#pragma once
#include "cheat/hook/javahook.hpp"

class GetMouseOver
{
public:
	static void GetMouseOverDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};
