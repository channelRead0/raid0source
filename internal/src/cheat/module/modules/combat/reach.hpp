#pragma once
#include "cheat/module/module.hpp"

class Reach : public Module
{
    RandomNumberGenerator Random;
public:

    inline static bool OnlySprinting = true;
    inline static bool LiquidCheck = true;
    inline static bool HitThroughWalls = true;
    //inline static bool AimAtInvisible = true;
    inline static bool YCheck = true;
    inline static bool ExpandHitbox = false;

    inline static float HitboxScaleMin = 1.F;
    inline static float HitboxScaleMax = 1.F;
    inline static float DistanceMax = 3.3F;
    inline static float DistanceMin = 3.1F;

    inline static int Chance = 90;

    virtual void OnStrictMathAtan2(JNIEnv* Env) override;
};