#include "../../module.hpp"

class FastPlace : public Module
{
private:
    RandomNumberGenerator Random;
public:
    inline static int TickDelay = 0;
    inline static bool Randomize = true;
    inline static bool BlocksOnly = true;
    inline static bool OnRmb = true;

    virtual void Run(JNIEnv* Env) override;
};