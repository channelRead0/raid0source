#pragma once

class OnSwapBuffersHook
{
public:
	static long __stdcall SwapBuffersDetour(JNIEnv*, jclass, jobject);
	static decltype(&SwapBuffersDetour) OriginalSwapBuffers;
};
