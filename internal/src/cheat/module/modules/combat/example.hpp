#pragma once
#include "../../module.hpp"

class Example : public Module
{
public:
    virtual void OnTick(JNIEnv* Env) override;
    virtual void OnPacketReceive(JNIEnv* Env, const std::shared_ptr<PacketReceiveData>& PacketData, bool* ShouldCancel) override;
};