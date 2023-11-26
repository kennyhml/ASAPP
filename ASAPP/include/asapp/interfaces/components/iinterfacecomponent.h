#pragma once
#include "../../game/window.h"

namespace asa::interfaces::components
{
	struct IInterfaceComponent
	{
		IInterfaceComponent() : IInterfaceComponent(0, 0, 0, 0){};
		IInterfaceComponent(window::Rect area) : area(area){};
		IInterfaceComponent(int x, int y, int width, int height)
			: area(window::Rect(x, y, width, height)){};

		window::Rect area;
	};

}