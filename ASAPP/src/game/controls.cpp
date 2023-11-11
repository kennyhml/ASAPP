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
	if (x >= 360 || x <= -360) {
		x %= 360;
	}
	if (y >= 90 || y <= -90) {
		y %= 90;
	}

	TurnPosition(x * pixelsPerDegree, y * pixelsPerDegree);
}

void controls::TurnPosition(int x, int y)
{
	INPUT input{ 0 };
	input.type = INPUT_MOUSE;

	input.mi.dx = x * GetLRFactor() * GetFovFactor();
	input.mi.dy = y * GetUDFactor() * GetFovFactor();
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_MOVE_NOCOALESCE;

	SendInput(1, &input, sizeof(INPUT));
}

void controls::TurnTo(int x, int y)
{
	INPUT input{ 0 };
	input.type = INPUT_MOUSE;
	int moveX = (x - GetSystemMetrics(SM_CXSCREEN) / 2);
	int moveY = (y - GetSystemMetrics(SM_CYSCREEN) / 2);

	input.mi.dx = moveX;
	input.mi.dy = moveY;
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_MOVE_NOCOALESCE;

	SendInput(1, &input, sizeof(INPUT));
}

controls::KeyboardMapping controls::GetKeyboardMapping()
{
	KeyboardMapping mapping{ { "tab", VK_TAB }, { "f1", VK_F1 },
		{ "f2", VK_F2 }, { "f3", VK_F3 }, { "f4", VK_F4 }, { "f5", VK_F5 },
		{ "f6", VK_F6 }, { "f7", VK_F7 }, { "f8", VK_F8 }, { "f9", VK_F9 },
		{ "f10", VK_F10 }, { "delete", VK_DELETE }, { "home", VK_HOME },
		{ "end", VK_END }, { "backspace", VK_BACK }, { "enter", VK_RETURN },
		{ "period", VK_OEM_PERIOD }, { "numpadzero", VK_NUMPAD0 },
		{ "numpadone", VK_NUMPAD1 }, { "numpadtwo", VK_NUMPAD2 },
		{ "numpadthree", VK_NUMPAD3 }, { "numpadfour", VK_NUMPAD4 },
		{ "numpadfive", VK_NUMPAD5 }, { "numpadsix", VK_NUMPAD6 },
		{ "numpadseven", VK_NUMPAD7 }, { "numpadeight", VK_NUMPAD8 },
		{ "NumPadnine", VK_NUMPAD9 }, { "ctrl", VK_CONTROL },
		{ "esc", VK_ESCAPE }, { "space ", VK_SPACE } };

	for (int i = 32; i < 128; i++) {
		char c = static_cast<char>(i);
		std::string character(1, c);
		mapping[character] = VkKeyScanA(i);
	}
	return mapping;
}

void controls::KeyDown(std::string key)
{
	INPUT input{ 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = controls::GetVirtualCode(key);

	SendInput(1, &input, sizeof(INPUT));
}

void controls::KeyUp(std::string key)
{
	INPUT input{ 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = controls::GetVirtualCode(key);
	input.ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(1, &input, sizeof(INPUT));
}

void controls::KeyPress(std::string key, float durationMs)
{
	KeyDown(key);
	Sleep(durationMs);
	KeyUp(key);
}

void controls::KeyCombinationPress(std::string holdKey, std::string pressKey)
{
	KeyDown(holdKey);
	Sleep(20);
	KeyPress(pressKey);
	Sleep(20);
	KeyUp(holdKey);
}

int controls::GetVirtualCode(std::string key)
{
	std::transform(key.begin(), key.end(), key.begin(),
		[](unsigned char c) { return std::tolower(c); });

	return mapping[key];
}