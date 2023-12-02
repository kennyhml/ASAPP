#include "asapp/game/controls.h"
#include "../core/wrappers.h"
#include <algorithm>

int constexpr asa::controls::GetMouseFlag(MouseButton button, bool down)
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

int asa::controls::GetVirtualKeyCode(std::string key)
{
	std::transform(key.begin(), key.end(), key.begin(),
		[](unsigned char c) { return std::tolower(c); });

	return mapping[key];
}

asa::controls::KeyboardMapping asa::controls::GetKeyboardMapping()
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
		{ "esc", VK_ESCAPE }, { "space", VK_SPACE }, { "spacebar", VK_SPACE } };

	for (int i = 32; i < 128; i++) {
		char c = static_cast<char>(i);
		std::string character(1, c);
		SHORT vkCode = VkKeyScanA(i);

		mapping[character] = VkKeyScanA(i);
	}
	return mapping;
}

const float asa::controls::GetLRFactor()
{
	return 3.2 / asa::settings::sensX.get();
}

const float asa::controls::GetUDFactor()
{
	return 3.2 / asa::settings::sensY.get();
}

const float asa::controls::GetFovFactor()
{
	return 1.25 / asa::settings::fov.get();
}


bool asa::controls::IsMouseInput(const settings::ActionMapping& input)
{
	return input.key.find("Mouse") != std::string::npos;
}

bool asa::controls::IsKeyInput(const settings::ActionMapping& input)
{
	return !IsMouseInput(input);
}

void asa::controls::Down(const settings::ActionMapping& input, ms delay)
{
	IsMouseInput(input) ? MouseDown(stringToMouseButton[input.key], delay)
						: KeyDown(input.key, delay);
}

void asa::controls::Release(const settings::ActionMapping& input, ms delay)
{
	IsMouseInput(input) ? MouseUp(stringToMouseButton[input.key], delay)
						: KeyUp(input.key, delay);
}

void asa::controls::Press(const settings::ActionMapping& input, ms delay)
{
	IsMouseInput(input) ? MousePress(stringToMouseButton[input.key], delay)
						: KeyPress(input.key, delay);
}
void asa::controls::MouseDown(MouseButton button, ms delay)
{
	INPUT input{ 0 };
	input.type = INPUT_MOUSE;

	input.mi.dwFlags = GetMouseFlag(button, true);

	if (button == MouseButton::MOUSE4) {
		input.mi.mouseData = XBUTTON1;
	}
	else if (button == MouseButton::MOUSE5) {
		input.mi.mouseData = XBUTTON2;
	}

	SendInput(1, &input, sizeof(INPUT));
	SleepFor(delay);
}

void asa::controls::MouseUp(MouseButton button, ms delay)
{
	INPUT input{ 0 };
	input.type = INPUT_MOUSE;

	input.mi.dwFlags = GetMouseFlag(button, false);

	if (button == MouseButton::MOUSE4) {
		input.mi.mouseData = XBUTTON1;
	}
	else if (button == MouseButton::MOUSE5) {
		input.mi.mouseData = XBUTTON2;
	}

	SendInput(1, &input, sizeof(INPUT));
	SleepFor(delay);
}

void asa::controls::MousePress(MouseButton button, ms delay)
{
	MouseDown(button);
	SleepFor(delay);
	MouseUp(button);
}

void asa::controls::MouseCombinationPress(MouseButton button, std::string key)
{
	KeyDown(key);
	SleepFor(ms(20));
	MousePress(button);
	SleepFor(ms(20));
	KeyUp(key);
}

void asa::controls::TurnDegrees(int x, int y)
{
	x %= 360;
	y %= 90;

	TurnPosition(x * pixelsPerDegree, y * pixelsPerDegree);
}

void asa::controls::TurnPosition(int x, int y)
{
	INPUT input{ 0 };
	input.type = INPUT_MOUSE;

	input.mi.dx = x * GetLRFactor() * GetFovFactor();
	input.mi.dy = y * GetUDFactor() * GetFovFactor();
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_MOVE_NOCOALESCE;

	SendInput(1, &input, sizeof(INPUT));
}

void asa::controls::TurnTo(int x, int y)
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

void asa::controls::KeyDown(std::string key, ms delay)
{
	INPUT input{ 0 };
	input.type = INPUT_KEYBOARD;
	int wVk = GetVirtualKeyCode(key);
	input.ki.wVk = wVk;
	if (HIBYTE(wVk) & 0x01) {
		INPUT shiftInput{ 0 };
		shiftInput.type = INPUT_KEYBOARD;
		shiftInput.ki.wVk = VK_SHIFT;
		SendInput(1, &shiftInput, sizeof(INPUT));
	}

	SendInput(1, &input, sizeof(INPUT));
	SleepFor(delay);
}

void asa::controls::KeyUp(std::string key, ms delay)
{
	INPUT input{ 0 };
	input.type = INPUT_KEYBOARD;
	int wVk = GetVirtualKeyCode(key);
	input.ki.wVk = wVk;
	input.ki.dwFlags = KEYEVENTF_KEYUP;

	if (HIBYTE(wVk) & 0x01) {
		INPUT shiftInput{ 0 };
		shiftInput.type = INPUT_KEYBOARD;
		shiftInput.ki.wVk = VK_SHIFT;
		shiftInput.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &shiftInput, sizeof(INPUT));
	}

	SendInput(1, &input, sizeof(INPUT));
	SleepFor(delay);
}

void asa::controls::KeyPress(std::string key, ms delay)
{
	KeyDown(key);
	SleepFor(delay);
	KeyUp(key);
}

void asa::controls::KeyCombinationPress(
	std::string holdKey, std::string pressKey)
{
	KeyDown(holdKey, ms(20));
	KeyPress(pressKey, ms(20));
	KeyUp(holdKey);
}