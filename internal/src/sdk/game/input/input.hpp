#pragma once

class Input
{
public:
	void PressMouse(bool Right) const;
	void ReleaseMouse(bool Right) const;

	void ClickMouse(bool Right, int Delay) const;
};

extern Input* InputPtr;