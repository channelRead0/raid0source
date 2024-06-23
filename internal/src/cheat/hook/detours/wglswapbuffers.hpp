#pragma once
#include <gl/GL.h>
#include <ext/minhook/MinHook.h>

#include "cheat/raid0.hpp"
#include "ext/imgui/imgui_impl_opengl3.h"
#include "ext/imgui/imgui_impl_win32.h"
#include "ext/imgui/imgui_internal.h"
#include "cheat/gui/ui.hpp"
inline bool uhhhlol = false;
inline BOOL(WINAPI* WglSwapBuffersOriginal)(HDC);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);

class WglSwapBuffersHook
{
public:
    static inline int UI_BIND = VK_INSERT;
    static inline bool InChat = false;
    static inline LONG_PTR WindowProcOriginal = NULL;

    static BOOL WINAPI DetourWglSwapBuffers(const HDC HandleContextDevice);
    static LRESULT WindowProcHooked(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);
};
