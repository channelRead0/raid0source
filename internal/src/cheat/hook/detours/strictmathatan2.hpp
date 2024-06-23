#pragma once

class StrictMathAtan2
{
public:
	static long __stdcall StrictMathAtan2Detour(JNIEnv*, jclass);
	static decltype(&StrictMathAtan2Detour) OriginalStrictMathAtan2;
};
