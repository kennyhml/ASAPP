#pragma once
#include "../../game/window.h"

namespace asa::interfaces::components
{
	struct IInterfaceComponent
	{
		IInterfaceComponent() : IInterfaceComponent(0, 0, 0, 0){};
		IInterfaceComponent(window::Rect t_area) : area(t_area){};
		IInterfaceComponent(int t_x, int t_y, int t_width, int t_height)
			: area(window::Rect(t_x, t_y, t_width, t_height)){};

		window::Rect area;
	};

}