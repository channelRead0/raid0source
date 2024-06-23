#pragma once
#include <sstream>

#include "cheat/hook/javahook.hpp"
#include "cheat/module/module.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include <ext/imgui/custom/custom.hpp>

class Particles : public Module
{
private:
	std::mutex PLock;
	std::vector<Vector3<double>> PositionRecords;
public:
	virtual void OnRender(JNIEnv* Env) override;
	virtual void OnRenderWorldPass(JNIEnv* Env, int Pass, float PartialTicks, long FinishTimeNano) override;
	virtual void Run(JNIEnv* Env) override;
};