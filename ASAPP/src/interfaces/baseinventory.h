#pragma once
#include "../game/window.h"
#include "components/button.h"
#include "components/combobox.h"
#include "components/searchbar.h"

#include "iinterface.h"

namespace asa::interfaces
{
	class BaseInventory : IInterface
	{
	public:
		BaseInventory(bool isRemote) : isRemoteInventory(isRemote)
		{
			using Point = window::Point;
			Point origin = isRemote ? Point(1176, 93) : Point(149, 93);

			this->SetArea(origin);
			this->itemFilter = { origin.x + 27, origin.y + 750, 552, 42 };
			this->searchBar = { origin.x + 29, origin.y + 84, 172, 44 };
		};

		components::ComboBox itemFilter;
		components::SearchBar searchBar;
		components::Button transferAllButton;

		window::Rect GetArea() const;

		bool IsOpen();

	private:
		bool isRemoteInventory{ false };
		window::Rect area;

		void SetArea(const window::Point& origin);
	};

}
