// ReSharper disable CppInconsistentNaming
#pragma once
#include <sdk/game/minecraft/packet.hpp>

#include "sdk/game/minecraft/channelhandlercontext.hpp"
#include "sdk/game/minecraft/networkmanager.hpp"
#include <sdk/game/minecraft/nethandlerplayclient.hpp>

struct PacketReceiveData
{
    std::shared_ptr<NetworkManager> NetworkManagerInstance;
    std::shared_ptr<ChannelHandlerContext> ChannelHandlerContextInstance;
    std::shared_ptr<Packet> PacketInstance;

    PacketReceiveData(const std::shared_ptr<NetworkManager>& NetworkManagerInstanceObject, const std::shared_ptr<ChannelHandlerContext>& ChannelHandlerContextInstanceObject, const std::shared_ptr<Packet>& PacketInstancePtr)
    {
        this->NetworkManagerInstance = NetworkManagerInstanceObject;
        this->ChannelHandlerContextInstance = ChannelHandlerContextInstanceObject;
        this->PacketInstance = PacketInstancePtr;
    }

    ~PacketReceiveData()
    {
        this->NetworkManagerInstance->UpdateInstanceObject(jobject());
        this->ChannelHandlerContextInstance->UpdateInstanceObject(jobject());
        this->PacketInstance->UpdateInstanceObject(jobject());
    }
};

struct PacketSendData
{
    std::shared_ptr<NetHandlerPlayClient> NetHandlerInstance;
    std::shared_ptr<Packet> PacketInstance;

    PacketSendData(const std::shared_ptr<NetHandlerPlayClient> NetHandlerPtr, const std::shared_ptr<Packet>& PacketInstancePtr)
    {
        this->NetHandlerInstance = NetHandlerPtr;
        this->PacketInstance = PacketInstancePtr;
    }
};

class Module {
private:
    int Keybind = 0;
    bool Enabled = false;

public:
    virtual ~Module() = default;

    virtual void Run(JNIEnv* Env) {}

    virtual void OnStrictMathAtan2(JNIEnv* Env) {}
    virtual void OnTick(JNIEnv* Env) {}
    virtual void OnImGuiRender(JNIEnv* Env) {}
    virtual void OnRender(JNIEnv* Env) {}
    virtual void OnGetMouseOver(JNIEnv* Env) {}
    virtual void OnUpdate(JNIEnv* Env) {}
    virtual void OnRespawn(JNIEnv* Env) {}
    virtual void OnUpdateCameraAndRender(JNIEnv* Env) {}
    virtual void OnLoadWorld(JNIEnv* Env, bool* ShouldCancel) {}
    virtual void OnRenderWorldPass(JNIEnv* Env, int Pass, float PartialTicks, long FinishTimeNano) {}
    virtual void OnSetAngles(JNIEnv* Env, float Yaw, float Pitch, bool* ShouldCancel) {}

    virtual void OnHandleStatusUpdate(JNIEnv* Env, const std::shared_ptr<Player>& TargetInstance, jbyte Byte, bool* ShouldCancel) {}

    virtual void OnPacketLocalEntityAttack(JNIEnv* Env, const std::shared_ptr<AttackPacket>& PacketData, bool* ShouldCancel) {}
    virtual void OnPacketUseEntity(JNIEnv* Env, const std::shared_ptr<PacketUseEntity>& PacketData, bool* ShouldCancel) {}
    virtual void OnPacketReceive(JNIEnv* Env, const std::shared_ptr<PacketReceiveData>& PacketData, bool* ShouldCancel) {}
    virtual void OnPacketSend(JNIEnv* Env, const std::shared_ptr<PacketSendData>& PacketData, bool* ShouldCancel) {}

    void UpdateState() {
        this->Enabled = !this->Enabled;
    }

    void Enable() {
        this->Enabled = true;
    }

    void Disable() {
        this->Enabled = false;
    }

    bool* GetTogglePtr() {
        return &this->Enabled;
    }

    int* GetBindPtr() {
        return &this->Keybind;
    }

    void SetKey(const int Keybind) {
        this->Keybind = Keybind;
    }

    int GetKey() const {
        return this->Keybind;
    }

    bool GetState() const {
        return this->Enabled;
    }
};

class ModuleManager {
private:
  /*  static std::vector<Module*> ModuleList;
    static std::vector<std::thread> ThreadList;
    static std::vector<Module*> ThreadedModuleList;*/
    static std::unordered_map<std::string, Module*> ModuleList;
    static std::unordered_map<std::string, Module*> ThreadedModuleList;
    static std::vector<std::thread> ThreadList;
public:
    inline static std::atomic<bool> SkipRead0Event = false;
    inline static std::atomic<bool> SkipAddToSendQueueEvent = false;
    inline static std::atomic<bool> SkipHandleStatusUpdateEvent = false;
    inline static std::atomic<bool> SkipSwingItemEvent = false;
    inline static std::atomic<bool> SkipSetAnglesEvent = false;

    static void RunAll(JNIEnv* env);

    static void RegisterModules();
    static void RegisterThreads();

    static void DestroyModules();
    static void DestroyThreads();

    static const auto& GetModuleByName(const char* Name)
    {
        std::string ModuleName(Name);

        auto It = ModuleList.find(ModuleName);
        if (It != ModuleList.end()) {
            return It->second;
        }

        It = ThreadedModuleList.find(ModuleName);
        if (It != ThreadedModuleList.end()) {
            return It->second;
        }

        return ThreadedModuleList.find({"Example"})->second;
    }

    static const auto& GetModules() {
        return ModuleList;
    }

    static const auto& GetThreadedModules() {
        return ThreadedModuleList;
    }

    static const auto& SkipRead0() {
        return SkipRead0Event;
    }

    static const auto& SkipHandleStatusUpdate() {
        return SkipHandleStatusUpdateEvent;
    }

    static const auto& SkipAddToSendQueue() {
        return SkipAddToSendQueueEvent;
    }

    static const auto& SkipSwingItem() {
        return SkipSwingItemEvent;
    }

    static const auto& SkipSetAngles() {
        return SkipSetAnglesEvent;
    }
};