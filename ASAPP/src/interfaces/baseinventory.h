#pragma once
#include "../game/window.h"
#include "components/combobox.h"
#include "components/searchbar.h"

#include "iasainterface.h"

namespace asa
{
	class BaseInventory : IASAInterface
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

		ASAComboBox itemFilter;
		ASASearchBar searchBar;

		window::Rect GetArea() const;

		bool IsOpen();

	private:
		bool isRemoteInventory{ false };
		window::Rect area;

		void SetArea(const window::Point& origin);
	};

}
