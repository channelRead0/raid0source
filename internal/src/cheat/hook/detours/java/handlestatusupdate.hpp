#pragma once
#include "cheat/hook/javahook.hpp"

class HandleStatusUpdate
{
public:
	static void HandleStatusUpdateDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};
