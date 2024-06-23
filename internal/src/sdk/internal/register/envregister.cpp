#include "pch.h"
#include "envregister.hpp"

JNIEnv* EnvRegister::GetThreadEnv()
{
	const auto CallingThreadID = GetCurrentThreadId();
	
	if (const auto EnvRegistered = RegisteredEnv::RegisteredEnv.find(CallingThreadID); EnvRegistered != RegisteredEnv::RegisteredEnv.end()) {
		return EnvRegistered->second;
	}

	return nullptr;
}

void EnvRegister::RegisterThreadEnv(JNIEnv* Env)
{
	//RegisteredEnv::EnvMutex.lock();

	const auto CallingThreadID = GetCurrentThreadId();

	if (const auto IsEnvRegistered = RegisteredEnv::RegisteredEnv.find(CallingThreadID); IsEnvRegistered != RegisteredEnv::RegisteredEnv.end()) {
		return;
	}

	//RegisteredEnv::EnvMutex.unlock();

	RegisteredEnv::RegisteredEnv[CallingThreadID] = Env;
}

void EnvRegister::DestroyThreadEnv()
{
	RegisteredEnv::EnvMutex.lock();

	const auto CallingThreadID = GetCurrentThreadId();

	if (const auto EnvRegistered = RegisteredEnv::RegisteredEnv.find(CallingThreadID); EnvRegistered != RegisteredEnv::RegisteredEnv.end()) {
		RegisteredEnv::RegisteredEnv.unsafe_erase(CallingThreadID);
	}

	RegisteredEnv::EnvMutex.unlock();
}
