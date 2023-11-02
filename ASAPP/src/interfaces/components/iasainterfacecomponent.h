#pragma once
#include "../../game/window.h"

namespace asa
{
	struct IASAInterfaceComponent
	{
		IASAInterfaceComponent() : IASAInterfaceComponent(0, 0, 0, 0){};
		IASAInterfaceComponent(window::Rect area) : area(area){};
		IASAInterfaceComponent(int x, int y, int width, int height)
			: area(window::Rect(x, y, width, height)){};

		window::Rect area;
	};

}