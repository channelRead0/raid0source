#pragma once
#include "cheat/hook/javahook.hpp"

class AltManager
{
private:
	jobject OriginalSession = nullptr;
public:
	void LoginOffline(JNIEnv* Env);
	void Reset(JNIEnv* Env);

	inline static bool UseCrack = false;

	static void SendPacketDetour(Frame* Frame, Thread* Thread, bool* ShouldCancel);
};

extern AltManager* AltManagerPtr;