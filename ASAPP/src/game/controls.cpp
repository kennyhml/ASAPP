#include "controls.h"

using namespace asa;

const float controls::GetLRFactor() { return 3.2 / asa::settings::sensX.get(); }

const float controls::GetUDFactor() { return 3.2 / asa::settings::sensY.get(); }

const float controls::GetFovFactor() { return 1.25 / asa::settings::fov.get(); }

int constexpr controls::GetMouseFlag(MouseButton button, bool down)
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

void controls::TurnDegrees(int x, int y)
{
	if (x > 360 || x < -360) {
		x %= 360;
	}
	if (y > 90 || y < -90) {
		y %= 90;
	}

	TurnPosition(x * pixelsPerDegree, y * pixelsPerDegree);
}

void controls::TurnPosition(int x, int y)
{
	LPPOINT pos{ 0 };
	GetCursorPos(pos);

	INPUT input{ 0 };
	input.type = INPUT_MOUSE;

	input.mi.dx = x * GetLRFactor() * GetFovFactor();
	input.mi.dy = y * GetUDFactor() * GetFovFactor();
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_MOVE_NOCOALESCE;

	SendInput(1, &input, sizeof(INPUT));
}

controls::KeyboardMapping controls::GetKeyboardMapping()
{
	KeyboardMapping mapping{ { "Tab", VK_TAB }, { "F1", VK_F1 },
		{ "F2", VK_F2 }, { "F3", VK_F3 }, { "F4", VK_F4 }, { "F5", VK_F5 },
		{ "F6", VK_F6 }, { "F7", VK_F7 }, { "F8", VK_F8 }, { "F9", VK_F9 },
		{ "F10", VK_F10 }, { "Delete", VK_DELETE }, { "Home", VK_HOME },
		{ "End", VK_END }, { "BackSpace", VK_BACK }, { "Enter", VK_RETURN },
		{ "Period", VK_OEM_PERIOD }, { "NumPadZero", VK_NUMPAD0 },
		{ "NumPadOne", VK_NUMPAD1 }, { "NumPadTwo", VK_NUMPAD2 },
		{ "NumPadThree", VK_NUMPAD3 }, { "NumPadFour", VK_NUMPAD4 },
		{ "NumPadFive", VK_NUMPAD5 }, { "NumPadSix", VK_NUMPAD6 },
		{ "NumPadSeven", VK_NUMPAD7 }, { "NumPadEight", VK_NUMPAD8 },
		{ "NumPadNine", VK_NUMPAD9 } };

	for (int i = 32; i < 128; i++) {
		char c = static_cast<char>(i);
		std::string character(1, c);
		mapping[character] = VkKeyScanA(i);
	}
	return mapping;
}

void controls::KeyDown(std::string key)
{
	int vk = mapping[key];

	INPUT input{ 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = vk;

	SendInput(1, &input, sizeof(INPUT));
}

void controls::KeyUp(std::string key)
{
	int vk = mapping[key];

	INPUT input{ 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = vk;
	input.ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(1, &input, sizeof(INPUT));
}

void controls::KeyPress(std::string key, float durationMs)
{
	KeyDown(key);
	Sleep(durationMs);
	KeyUp(key);
}