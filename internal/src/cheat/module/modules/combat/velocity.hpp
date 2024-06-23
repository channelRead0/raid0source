#pragma once
#include <cheat/module/module.hpp>

class Velocity : public Module
{
    RandomNumberGenerator Random;
    std::atomic<DWORD> LastAttackTime = 0;
    std::atomic<bool> DoReset = false;
public:
    enum VELOCITY_MODE
    {
        NORMAL = 0,
        PACKET = 1,
    };

    inline static bool LiquidCheck = true;
    inline static bool CheckOnFire = true;
    inline static bool OnlyTargeting = false;
    inline static bool OnlySprinting = false;
    inline static int VelocityMode = NORMAL;
    inline static float VelocityVertical = 100.F;
    inline static float VelocityHorizontal = 100.F;
    inline static bool UseDelay = false;
    inline static int Chance = 100;
    inline static int Delay = 1;

    virtual void Run(JNIEnv* Env) override;
    virtual void OnPacketReceive(JNIEnv* Env, const std::shared_ptr<PacketReceiveData>& PacketData, bool* ShouldCancel) override;
};