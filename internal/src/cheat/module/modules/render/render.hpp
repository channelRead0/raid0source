#pragma once
#include <sstream>

#include "cheat/hook/javahook.hpp"
#include "cheat/module/module.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include <ext/imgui/custom/custom.hpp>

class Render : public Module
{
public:

	enum RENDER_MODES
	{
		PLAYERS = 0,
		GROUND_ITEMS = 2,
		MOBS = 2,
		ALL = 3,
	};

	inline static int RenderMode = ALL;
	inline static float RenderDistance = 20.F;
    inline static float FadeDistance = 6.F;
    inline static bool UniformScaling = false;
    inline static bool ExcludeTeamMates = true;
    inline static bool ExcludeFriends = true;

	virtual void Run(JNIEnv* Env) override;
    static void RenderName(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};

class PlayerESP : public Module
{
public:

    inline static bool DrawOutlines = true;
    inline static bool DrawESPBackground = true;
    inline static bool DrawESPOutlines = true;
    inline static bool DrawESPShadow = true;
    inline static bool DrawHealthBar = true;

    inline static ImVec4 OutlineColor = Color(0, 0, 0, 255).GetColorVec4();
    inline static ImVec4 EspOutlineColor = Color(114, 85, 137, 255).GetColorVec4();
    inline static ImVec4 EspBackgroundColor = Color(0, 0, 0, 125).GetColorVec4();
    inline static ImVec4 HealthBarBackgroundColor = Color(51, 51, 51, 255).GetColorVec4();
    inline static ImVec4 HealthBarShadedColor = Color(135, 121, 63, 255).GetColorVec4();

    enum RENDER_MODE
    {
        RENDER_2D = 0,
        RENDER_3D = 1
    };

    enum DRAW_STYLE
    {
        FULL = 0,
        CORNERS = 1
    };
    //todo recolor visuals and ignore teammates
    inline static int RenderMode = RENDER_2D;
    inline static int RenderStyle = FULL;

	virtual void OnImGuiRender(JNIEnv* Env) override;
    virtual void OnRender(JNIEnv* Env) override;
};

class ItemESP : public Module
{
public:
	virtual void OnImGuiRender(JNIEnv* Env) override;
};

class NameTags : public Module
{
public:
};

class Radar : public Module
{
public:
    inline static float RadarZoom = 4.7F;

    inline static ImVec4 BlipColor = Color(114, 85, 137, 255).GetColorVec4();

    virtual void OnImGuiRender(JNIEnv* Env) override;
};

namespace RenderData
{
    struct PlayerRenderInfo
    {
        std::string EntityName;

        double Distance;
        double Health;
        AxisAlignedBB_T<double> BoundingBoxData;
    };

    struct ItemRenderInfo
    {
        double Distance;
        AxisAlignedBB_T<double> BoundingBoxData;
    };

    struct RadarRenderInfo
    {
        Vector3<double> PlayerCoordinates;
    };

    struct EntityRenderInfo
    {

    };

    struct RenderInfo
    {
        std::string EntityName;

        AxisAlignedBB_T<double> HitboxData3D;
    };

    inline std::mutex RenderDataMutex;
}