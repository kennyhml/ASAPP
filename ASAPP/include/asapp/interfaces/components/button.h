#pragma once
#include "iinterfacecomponent.h"
#include <chrono>

namespace asa::interfaces::components
{
	struct Button : IInterfaceComponent
	{
	public:
		inline static std::chrono::system_clock::time_point last_button_press =
			std::chrono::system_clock::now();

		Button(window::Rect t_area, int t_padding = 2);
		Button(int t_x, int t_y, int t_width, int t_height, int t_padding = 2);

		virtual void press();

		std::chrono::system_clock::time_point get_last_press() const
		{
			return last_pressed;
		}

		int get_padding() const { return padding; }

	protected:
		std::chrono::system_clock::time_point last_pressed;
		int padding;
	};
}