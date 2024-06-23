#pragma once
#include <random>

namespace Utils
{
	inline FILE* ConsoleFilePointer;
	inline void AllocateConsole()
	{
		AllocConsole();
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);
	}

	inline void DeAllocateConsole()
	{
		if (!GetConsoleWindow()) {
			return;
		}

		ShowWindow(GetConsoleWindow(), SW_HIDE);
		FreeConsole();
	}

	inline std::string GetWindowName(const HWND Handle)
	{
		char WindowName[500];
		GetWindowTextA(Handle, WindowName, 500);

		return std::string(WindowName);
	}
}

