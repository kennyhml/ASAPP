#pragma once
#include "iasainterfacecomponent.h"

namespace asa
{
	struct ASAButton : IASAInterfaceComponent
	{
		using IASAInterfaceComponent::IASAInterfaceComponent;

		ASAButton(window::Rect area, int padding = 2)
			: borderPadding(padding), IASAInterfaceComponent(area){};

		ASAButton(int x, int y, int width, int height, int padding = 2)
			: borderPadding(padding),
			  IASAInterfaceComponent(x, y, width, height){};

		int borderPadding;

		virtual void Press(){};
	};
}