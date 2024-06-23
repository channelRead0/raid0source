#include "../../module.hpp"

class FrameSpoofer : public Module
{
public:
    inline static bool Toggled = false;
    inline static float Multiplier = 1.F;
    virtual void OnTick(JNIEnv* Env) override;
};