#pragma once
#include "cheat/module/module.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"

class AimAssist : public Module
{
private:
    RandomNumberGenerator Random;

    std::atomic<float> YawChange;
    std::atomic<float> PitchChange;
public:
    enum FILTERING_MODE
    {
    	CLOSEST = 0,
        LOWEST_HEALTH = 1,
        FOV = 2
    };

    enum TARGETING_MODE
    {
	    SWITCH = 0,
        SINGLE = 1
    };

    enum AIM_MODE
    {
	    MULTIPOINT = 0,
        STATIC = 1
    };

	enum AIM_DIRECTION
    {
	    VERTICAL = 0,
        HORIZONTAL = 1,
        BOTH = 2
    };

    enum SMOOTHING
    {
        LINEAR = 0,
        CONSTANT = 1,
        EASE_OUT_CUBIC = 2,
        EASE_IN_QUART = 3
    };

    enum VERTICAL_POINT
    {
        EYES = 0,
        CORNER = 1
    };

    inline static bool ItemWhitelist = false;//done

    inline static bool WhitelistSwords = false;//
    inline static bool WhitelistAxes = false;//
    inline static bool WhitelistFist = false;//
    inline static bool WhitelistSticks = false;//
    inline static bool WhitelistPickAxe = false;//

    inline static bool OnlyClicking = true;//done
    inline static bool AimThroughWalls = false;//todo
    inline static bool AimAtInvisible = false;//done
    inline static bool BreakBlocks = false;//todo
   // inline static bool FreeYAxisInHitbox = false;//done
    inline static bool Vertical = false;//done

    inline static int VerticalPoint = EYES;
    inline static int TargetingMode = SWITCH;
    inline static int AimMode = STATIC;
	inline static int FilteringMode = FOV;
	inline static int AimDirection = BOTH;
    inline static int Smoothing = LINEAR;

    inline static int RandomizationScale = 100.F;
    inline static int RandomizationMagnitude = 100.F;

    inline static float VerticalSpeed = 10.F;
    inline static float HorizontalSpeed = 10.F;
    inline static float AngleChange = 5.F;
    inline static float MinimumFov = 0.F;//done
    inline static float MaximumFov = 180.F;
    inline static float MultiPointOffset = 1.F;//done
    inline static float MaximumRange = 6.0F;
    inline static float MinimumRange = 0.0F;//done

    virtual void Run(JNIEnv* Env) override;
   // virtual void OnRenderWorldPass(JNIEnv* Env, int Pass, float PartialTicks, long FinishTimeNano) override;
    //virtual void OnUpdate(JNIEnv* Env);
   // virtual void OnSetAngles(JNIEnv* Env, float Yaw, float Pitch, bool* ShouldCancel) override;
};
