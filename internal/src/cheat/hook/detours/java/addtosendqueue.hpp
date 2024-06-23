#pragma once

#include "cheat/hook/javahook.hpp"

class AddToSendQueue
{
public:
	static void AddToSendQueueDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};
