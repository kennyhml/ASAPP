#pragma once
#include "iinterfacecomponent.h"
#include <chrono>

namespace asa::interfaces::components
{
	struct Button : IInterfaceComponent
	{
		using TimeStamp = std::chrono::time_point<std::chrono::system_clock,
			std::chrono::seconds>;

		inline static TimeStamp lastButtonPress =
			std::chrono::time_point_cast<std::chrono::seconds>(
				std::chrono::system_clock::now());

		Button() : Button(0, 0, 0, 0){};
		Button(window::Rect area, int padding = 2)
			: borderPadding(padding), IInterfaceComponent(area){};

		Button(int x, int y, int width, int height, int padding = 2)
			: borderPadding(padding),
			  IInterfaceComponent(x, y, width, height){};

		int borderPadding;

		std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>
			lastPress;

		virtual void Press()
		{
			this->lastPress =
				std::chrono::time_point_cast<std::chrono::seconds>(
					std::chrono::system_clock::now());
			this->lastButtonPress = lastPress;
		};
	};
}