#pragma once

#include "cheat/hook/javahook.hpp"
#include "src/cheat/module/module.hpp"

class UpdateCameraAndRender
{
public:
    static void UpdateCameraAndRenderDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};
