#pragma once
#include <string>

namespace asa::controls
{
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
	void MousePress(MouseButton button);

	void KeyDown(std::string key);
	void KeyUp(std::string key);
	void KeyPress(std::string key);

	int GetScanCode(std::string key);
}
