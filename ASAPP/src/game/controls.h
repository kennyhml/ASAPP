#pragma once
#include <Windows.h>
#include <string>
#include <unordered_map>

namespace asa::controls
{
	using KeyboardMapping = std::unordered_map<std::string, int>;

	enum MouseButton
	{
		LEFT,
		RIGHT,
		MIDDLE,
		MOUSE4,
		MOUSE5,
	};

	void MouseDown(MouseButton button);
	void MouseUp(MouseButton button);
	void MousePress(MouseButton button, float durationMs = 50);
	int constexpr GetMouseFlag(MouseButton button, bool down);

	KeyboardMapping GetKeyboardMapping();
	inline KeyboardMapping mapping = GetKeyboardMapping();

	void KeyDown(std::string key);
	void KeyUp(std::string key);
	void KeyPress(std::string key, float durationMs = 50);
	int GetVirtualCode(std::string key);
}
