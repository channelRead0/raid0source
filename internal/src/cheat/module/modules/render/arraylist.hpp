#pragma once
#include <sstream>

#include "cheat/hook/javahook.hpp"
#include "cheat/module/module.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include <ext/imgui/custom/custom.hpp>

class ArrayList : public Module
{
public:
	inline static ImVec4 FlagColor = Color(150, 150, 150, 255).GetColorVec4();
	inline static ImVec4 ModuleColor = Color(158, 120, 160).GetColorVec4();
	inline static ImVec4 ModuleColorB = Color(91, 69, 91, 255).GetColorVec4();
	inline static ImVec4 ModuleBoxColor = Color(35, 34, 34, 255).GetColorVec4();

	enum COLOR_MODE
	{
		STATIC = 0,
		CHROMA = 1,
		STATIC_COLORWAVE = 2
	};
	
	enum FONT_MODE
	{
		FONT_A = 0,
		FONT_B = 1,
		FONT_C = 2,
	};

	inline static int Font = FONT_A;
	inline static int ColorMode = STATIC_COLORWAVE;
	inline static float Saturation = .6F;
	inline static bool Bar = true;
	inline static bool TextShadows = true;
	inline static bool LowerCase = true;
	inline static bool InverseColorWave = false;
	inline static bool InverseColorWaveDirection = false;
	inline static bool StaticColorWave = true;
	inline static bool DrawBox = true;

	inline static float WaveSpeed = .5F;

	inline static float PaddingX = 3.F;
	inline static float PaddingY = -1.F;
	inline static float BarWidth = 2.F;
	inline static float Rounding = 3.F;

	inline static float StartOffsetX = 0.F;
	inline static float StartOffsetY = 0.F;

	virtual void OnImGuiRender(JNIEnv* Env) override;
};