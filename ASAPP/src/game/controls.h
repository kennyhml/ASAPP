#pragma once
#include "settings.h"
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

	const float pixelsPerDegree = 129.f / 90.f;
	const float GetLRFactor();
	const float GetUDFactor();
	const float GetFovFactor();
	int constexpr GetMouseFlag(MouseButton button, bool down);

	void MouseDown(MouseButton button);
	void MouseUp(MouseButton button);
	void MousePress(MouseButton button, float durationMs = 50);
	void TurnDegrees(int x, int y);
	void TurnPosition(int x, int y);
	void TurnTo(int x, int y);

	KeyboardMapping GetKeyboardMapping();
	inline KeyboardMapping mapping = GetKeyboardMapping();

	void KeyDown(std::string key);
	void KeyUp(std::string key);
	void KeyPress(std::string key, float durationMs = 50);
	int GetVirtualCode(std::string key);
}
