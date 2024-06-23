#include "pch.h"
#include "onwindowsdisplayupdate.hpp"
#include <cheat/module/module.hpp>

decltype(OnUpdate::OriginalOnUpdate) OnUpdate::OriginalOnUpdate;
long __stdcall OnUpdate::OnUpdateDetour(JNIEnv* Env, jclass CallingClass)
{
    for (const auto& [Name, Mod] : ModuleManager::GetModules())
    {
        Env->PushLocalFrame(100);
        Mod->OnUpdate(Env);
        Env->PopLocalFrame(NULL);
    }

    for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
    {
        Env->PushLocalFrame(100);
        Mod->OnUpdate(Env);
        Env->PopLocalFrame(NULL);
    }

    return OriginalOnUpdate(Env, CallingClass);
}
