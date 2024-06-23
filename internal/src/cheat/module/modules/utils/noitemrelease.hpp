#pragma once
#include "../../module.hpp"

class NoItemRelease : public Module
{
public:
    inline static bool Toggled = false;
    virtual void OnPacketSend(JNIEnv* Env, const std::shared_ptr<PacketSendData>& PacketData, bool* ShouldCancel) override;
};