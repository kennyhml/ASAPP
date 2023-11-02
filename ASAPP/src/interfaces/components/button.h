#pragma once
#include "iinterfacecomponent.h"

namespace asa::interfaces::components
{
	struct Button : IInterfaceComponent
	{
		Button() : Button(0, 0, 0, 0){};
		Button(window::Rect area, int padding = 2)
			: borderPadding(padding), IInterfaceComponent(area){};

		Button(int x, int y, int width, int height, int padding = 2)
			: borderPadding(padding),
			  IInterfaceComponent(x, y, width, height){};

		int borderPadding;

		virtual void Press(){};
	};
}