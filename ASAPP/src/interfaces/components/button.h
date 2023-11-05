#pragma once
#include "iinterfacecomponent.h"
#include <chrono>

namespace asa::interfaces::components
{
	struct Button : IInterfaceComponent
	{
		inline static std::chrono::system_clock::time_point lastButtonPress =
			std::chrono::system_clock::now();

		Button() : Button(0, 0, 0, 0){};
		Button(window::Rect area, int padding = 2)
			: borderPadding(padding), IInterfaceComponent(area){};

		Button(int x, int y, int width, int height, int padding = 2)
			: borderPadding(padding),
			  IInterfaceComponent(x, y, width, height){};

		int borderPadding;

		std::chrono::system_clock::time_point lastPressed;

		virtual void Press();
	};
}