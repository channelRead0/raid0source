// ReSharper disable CppInconsistentNaming
#pragma once
#include "pch.h"
#include "minhook/MinHook.h"

class Hook
{
private:
    struct HookData
    {
        void* OriginalFunction;
        void* TargetFunction;
    };

    std::vector<HookData> Hooks;
public:
    Hook() = default;

    bool Initialize() const {
        return MH_Initialize() == MH_OK;
    }

    template<typename T>
    bool CreateHook(LPVOID Target, LPVOID Detour, T* Original)
    {
	    const MH_STATUS Status = MH_CreateHook(Target, Detour, reinterpret_cast<LPVOID*>(Original));
        if (Status != MH_OK) 
        {
#ifdef SHOW_CONSOLE
            std::cerr << "Failed to create hook: " << MH_StatusToString(Status) << std::endl;
#endif
            return false;
        }

        const MH_STATUS HookStatus = MH_EnableHook(Target);
        if (HookStatus != MH_OK)
        {
            MH_RemoveHook(Target);

#ifdef SHOW_CONSOLE
            std::cerr << "Failed to enable hook: " << MH_StatusToString(HookStatus) << std::endl;
#endif
            return false;
        }

	    const HookData HookInfo
        {
            reinterpret_cast<void*>(Original),
            reinterpret_cast<void*>(Target),
        };

        Hooks.push_back(HookInfo);

        return true;
    }

    void DestroyHook()
    {
	    for (const auto [OriginalFunction, TargetFunction] : Hooks)
	    {
            const MH_STATUS HookStatus = MH_DisableHook(TargetFunction);
            if (HookStatus != MH_OK) {
#ifdef SHOW_CONSOLE
                std::cerr << "Failed to disable hook: " << MH_StatusToString(HookStatus) << std::endl;
#endif
            }

            const MH_STATUS Status = MH_RemoveHook(TargetFunction);
            if (Status != MH_OK) {
#ifdef SHOW_CONSOLE
                std::cerr << "Failed to remove hook: " << MH_StatusToString(Status) << std::endl;
#endif
            }
	    }

        Hooks.clear();
        MH_Uninitialize();
    }
};

inline std::unique_ptr<Hook> APIHookManager;