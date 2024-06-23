#pragma once

class OnUpdate
{
public:
	static long __stdcall OnUpdateDetour(JNIEnv*, jclass);
	static decltype(&OnUpdateDetour) OriginalOnUpdate;
};
