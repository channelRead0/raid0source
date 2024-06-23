#pragma once
#include "cheat/hook/javahook.hpp"
#include "sdk/game/minecraft/packet.hpp"

class ChannelRead0
{
public:
	static void ChannelRead0Detour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};
