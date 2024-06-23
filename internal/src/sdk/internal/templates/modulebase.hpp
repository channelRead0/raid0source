#pragma once
#include <src/cheat/module/module.hpp>

class ModuleBase : public Module
{
public:
    virtual void OnTick(JNIEnv* Env) override;
    virtual void OnPacketReceive(JNIEnv* Env, const std::shared_ptr<PacketReceiveData>& PacketData, bool* ShouldCancel) override;
};