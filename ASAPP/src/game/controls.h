#pragma once
#include "settings.h"
#include <Windows.h>
#include <chrono>
#include <string>

namespace asa::controls
{
	using ms = std::chrono::milliseconds;
	using KeyboardMapping = std::unordered_map<std::string, int>;

	enum MouseButton
	{
		LEFT,
		RIGHT,
		MIDDLE,
		MOUSE4,
		MOUSE5,
	};

	auto stringToMouseButton = std::unordered_map<std::string, MouseButton>{
		{ "LeftMouseButton", MouseButton::LEFT },
		{ "RightMouseButton", MouseButton::RIGHT },
		{ "MiddleMouseButton", MouseButton::MIDDLE },
		{ "ThumbMouseButton", MouseButton::MOUSE4 },
		{ "ThumbMouseButton2", MouseButton::MOUSE5 },
	};

	int constexpr GetMouseFlag(MouseButton button, bool down);
	int constexpr GetVirtualKeyCode(std::string key);

	KeyboardMapping GetKeyboardMapping();

	inline KeyboardMapping mapping = GetKeyboardMapping();

	const float pixelsPerDegree = 129.f / 90.f;
	const float GetLRFactor();
	const float GetUDFactor();
	const float GetFovFactor();

	bool IsMouseInput(const settings::ActionMapping&);
	bool IsKeyInput(const settings::ActionMapping&);

	void Down(const settings::ActionMapping&, ms delay = ms(10));
	void Release(const settings::ActionMapping&, ms delay = ms(10));
	void Press(const settings::ActionMapping&, ms delay = ms(150));

	void MouseDown(MouseButton, ms delay = ms(10));
	void MouseUp(MouseButton, ms delay = ms(10));
	void MousePress(MouseButton, ms delay = ms(150));

	void MouseCombinationPress(MouseButton, std::string key);
	void LeftClick();
	void RightClick();

	void TurnDegrees(int x, int y);
	void TurnPosition(int x, int y);
	void TurnTo(int x, int y);

	void KeyDown(std::string key, ms delay = ms(10));
	void KeyUp(std::string key, ms delay = ms(10));
	void KeyPress(std::string key, ms delay = ms(150));
	void KeyCombinationPress(std::string holdKey, std::string pressKey);
}
