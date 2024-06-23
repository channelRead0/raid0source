#pragma once
#include "sdk/game/minecraft/minecraft.hpp"
#include "sdk/game/minecraft/world.hpp"

class EnvRegister
{
public:
	static JNIEnv* GetThreadEnv();
	static void RegisterThreadEnv(JNIEnv* Env);
	static void DestroyThreadEnv();
};

namespace RegisteredEnv
{
	inline std::mutex EnvMutex;
	inline concurrency::concurrent_unordered_map<DWORD, JNIEnv*> RegisteredEnv;
}