#pragma once

#include "cheat/hook/javahook.hpp"
#include "src/cheat/module/module.hpp"

class RunTick
{
public:
    static void RunTickDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};
