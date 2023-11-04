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
	protected:
		struct ManagementButton : components::Button
		{
			ManagementButton() : Button(0, 0, 0, 0){};
			ManagementButton(int x, int y) : Button(x, y, 45, 45){};

			[[nodiscard]] bool IsToggled() const;
			[[nodiscard]] bool IsAvailable() const;
		};

		struct InvTabButton : components::Button
		{
			using components::Button::Button;

			[[nodiscard]] bool IsSelected() const;
			[[nodiscard]] bool Exists() const;
		};

	public:
		BaseInventory(bool isRemote) : isRemoteInventory(isRemote)
		{
			using Point = window::Point;
			Point org = isRemote ? Point(1176, 93) : Point(149, 93);

			this->SetArea(org);
			this->itemFilter = { org.x + 27, org.y + 750, 552, 42 };
			this->searchBar = { org.x + 29, org.y + 84, 172, 44 };
			this->transferAllButton = ManagementButton(org.x + 218, org.y + 84);
			this->dropAllButton = ManagementButton(org.x + 265, org.y + 84);
			this->newFolderButton = ManagementButton(org.x + 361, org.y + 84);
			this->autoStackButton = ManagementButton(org.x + 408, org.y + 84);
			this->folderViewButton = ManagementButton(org.x + 481, org.y + 84);
		};

		enum Tab
		{
			INVENTORY,
		};

		components::ComboBox itemFilter;
		components::SearchBar searchBar;

		ManagementButton transferAllButton;
		ManagementButton dropAllButton;
		ManagementButton newFolderButton;
		ManagementButton autoStackButton;
		ManagementButton folderViewButton;

		window::Rect GetArea() const;

		bool IsOpen();

	private:
		bool isRemoteInventory{ false };
		window::Rect area;

		void SetArea(const window::Point& origin);
	};

}
