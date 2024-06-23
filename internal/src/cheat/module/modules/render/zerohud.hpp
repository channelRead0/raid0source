#pragma once
#include <sstream>

#include "cheat/hook/javahook.hpp"
#include "cheat/module/module.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include <ext/imgui/custom/custom.hpp>

class ZeroHud : public Module
{
public:
	virtual void OnImGuiRender(JNIEnv* Env) override;
};