#include "controls.h"
#include <Windows.h>

using namespace asa;

int controls::GetMouseFlag(MouseButton button, bool down)
{
	switch (button) {

	case MouseButton::LEFT:
		return down ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
	case MouseButton::RIGHT:
		return down ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
	case MouseButton::MIDDLE:
		return down ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
	case MouseButton::MOUSE4:
	case MouseButton::MOUSE5:
		return down ? MOUSEEVENTF_XDOWN : MOUSEEVENTF_XUP;
	}

	return -1;
}

void controls::MouseDown(MouseButton button)
{
	INPUT input{ 0 };
	input.type = INPUT_MOUSE;

	input.mi.dwFlags = GetMouseFlag(button, true);

	if (button == MOUSE4 || button == MOUSE5) {
		input.mi.mouseData = button == MOUSE4 ? XBUTTON1 : XBUTTON2;
	}

	SendInput(1, &input, sizeof(INPUT));
}

void controls::MouseUp(MouseButton button)
{
	INPUT input{ 0 };
	input.type = INPUT_MOUSE;

	input.mi.dwFlags = GetMouseFlag(button, false);

	if (button == MOUSE4 || button == MOUSE5) {
		input.mi.mouseData = button == MOUSE4 ? XBUTTON1 : XBUTTON2;
	}

	SendInput(1, &input, sizeof(INPUT));
}

void controls::MousePress(MouseButton button, float durationMs)
{
	MouseDown(button);
	Sleep(durationMs);
	MouseUp(button);
}