#include "pch.h"
#include "input.hpp"

void Input::PressMouse(bool Right) const
{
	SendMessageW(WINDOW_HANDLE, Right ? WM_RBUTTONDOWN : WM_LBUTTONDOWN, Right ? MK_RBUTTON : MK_LBUTTON, MAKELPARAM(0, 0));
}

void Input::ReleaseMouse(bool Right) const
{
	SendMessageW(WINDOW_HANDLE, Right ? WM_RBUTTONUP : WM_LBUTTONUP, Right ? MK_RBUTTON : MK_LBUTTON, MAKELPARAM(0, 0));
}

void Input::ClickMouse(bool Right, int Delay) const
{
	SendMessageW(WINDOW_HANDLE, Right ? WM_RBUTTONDOWN : WM_LBUTTONDOWN, Right ? MK_RBUTTON : MK_LBUTTON, MAKELPARAM(0, 0));
	std::this_thread::sleep_for(std::chrono::milliseconds(Delay));
	SendMessageW(WINDOW_HANDLE, Right ? WM_RBUTTONUP : WM_LBUTTONUP, Right ? MK_RBUTTON : MK_LBUTTON, MAKELPARAM(0, 0));
}

Input* InputPtr = new Input();