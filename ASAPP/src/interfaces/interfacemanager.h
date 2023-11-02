#pragma once
#include "../game/window.h"
#include "components/button.h"
#include "iasainterface.h"

namespace asa
{
	class ASAInterfaceManager : IASAInterface
	{
		struct ASATabButton : ASAButton
		{
			ASATabButton(int x) : ASAButton(x, 53, 47, 48){};

			using ASAButton::ASAButton;
			bool IsSelected() const;
		};

	public:
		ASAButton closeButton{ 1784, 50, 32, 31 };

		ASATabButton inventoryTabButton{ 794 };
		ASATabButton engramsTabButton{ 851 };
		ASATabButton tribeTabButton{ 908 };
		ASATabButton trackingTabButton{ 965 };
		ASATabButton notesTabButton{ 1022 };
		ASATabButton mapTabButton{ 1079 };



		IASAInterface* GetOpenInterface() const;

		bool IsOpen();
	};

}