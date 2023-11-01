#pragma once
#include "../game/window.h"
#include "components/button.h"
#include "iasainterface.h"

namespace asa
{
	// needs all tabs: inventory, engrams, tribe, tracking, notes & map
	// also player stats  & dino stat tab, personal inventory & other inventory
	class ASAInterfaceManager : IASAInterface
	{
		struct ASATabButton : ASAButton
		{
			using ASAButton::ASAButton;
			bool IsSelected() const;
		};

	public:
		ASAButton closeButton{ 1784, 50, 32, 31 };

		bool IsOpen();
	};

}