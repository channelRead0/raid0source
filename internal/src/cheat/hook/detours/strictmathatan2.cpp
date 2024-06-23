#include "pch.h"
#include "strictmathatan2.hpp"
#include <cheat/module/module.hpp>

decltype(StrictMathAtan2::OriginalStrictMathAtan2) StrictMathAtan2::OriginalStrictMathAtan2;
long __stdcall StrictMathAtan2::StrictMathAtan2Detour(JNIEnv* Env, jclass CallingClass)
{
    for (const auto& [Name, Mod] : ModuleManager::GetModules())
    {
        Env->PushLocalFrame(100);
        Mod->OnStrictMathAtan2(Env);
        Env->PopLocalFrame(NULL);
    }

    for (const auto& [Name, Mod] : ModuleManager::GetThreadedModules())
    {
        Env->PushLocalFrame(100);
        Mod->OnStrictMathAtan2(Env);
        Env->PopLocalFrame(NULL);
    }

    return OriginalStrictMathAtan2(Env, CallingClass);
}
