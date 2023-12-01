#pragma once
#include "asapp/game/window.h"
#include "components/button.h"
#include "iinterface.h"

namespace asa::interfaces
{
	class InterfaceManager : IInterface
	{
		struct TabButton : components::Button
		{
			TabButton(int x) : Button(x, 53, 47, 48){};

			using Button::Button;
			bool IsSelected() const;
		};

	public:
		components::Button closeButton{ 1784, 50, 32, 31 };

		TabButton inventoryTabButton{ 794 };
		TabButton engramsTabButton{ 851 };
		TabButton tribeTabButton{ 908 };
		TabButton trackingTabButton{ 965 };
		TabButton notesTabButton{ 1022 };
		TabButton mapTabButton{ 1079 };

		IInterface* GetOpenInterface() const;

		virtual const bool IsOpen() const;
	};

	inline InterfaceManager* gInterfaceManager = new InterfaceManager();
}