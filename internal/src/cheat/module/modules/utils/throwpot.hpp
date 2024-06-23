// ReSharper disable CppInconsistentNaming
#pragma once
#include <numeric>

#include "cheat/module/module.hpp"

class ThrowPot : public Module
{
private:
    RandomNumberGenerator Random;
public:
    enum THROW_MODE
    {
        DYNAMIC = 0,
        SINGLE = 1,
        DOUBLE = 2,
    };


    inline static bool RandomSlots = false;
    inline static int ThrowMode = DOUBLE;
    inline static int ThrowBind = 0;
    inline static int TargetHealth = 100.F;
    inline static int ThrowDelay = 50.F;
    inline static int SwitchDelay = 50.F;

    virtual void Run(JNIEnv* Env) override;
};