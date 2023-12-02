#pragma once
#include "asapp/game/window.h"
#include "asapp/items/items.h"
#include "components/button.h"
#include "components/combobox.h"
#include "components/searchbar.h"
#include "iinterface.h"
#include <array>

namespace asa::interfaces
{
	const int MAX_ITEMS_PER_PAGE = 36;

	class BaseInventory : public IInterface
	{
	protected:
		struct ManagementButton : components::Button
		{
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

		struct Slot : window::Rect
		{
			Slot() : Slot(0, 0){};
			Slot(int x, int y) : Rect{ x, y, 86, 87 } {};

			[[nodiscard]] bool HasItem() const;
			[[nodiscard]] bool HasItem(items::Item* item) const;
		};

		ManagementButton transferAllButton;
		ManagementButton dropAllButton;
		ManagementButton newFolderButton;
		ManagementButton autoStackButton;
		ManagementButton folderViewButton;

		bool isRemoteInventory;
		window::Rect area;
		window::Rect recvRemoteInventoryArea{ 1340, 511, 295, 34 };

	public:
		BaseInventory(bool isRemote);

		enum Tab
		{
			INVENTORY,
		};

		components::ComboBox itemFilter;
		components::SearchBar searchBar;

		window::Rect itemArea;

		std::array<Slot, 36> slots;

		window::Rect GetArea() const;

		bool IsReceivingRemoteInventory() const;
		void ReceiveRemoteInventory(std::chrono::seconds timeout) const;

		virtual const bool IsOpen() const override;
		virtual bool Has(items::Item* item, bool search = false);
		virtual bool CountStacks(
			items::Item* item, int& stacksOut, bool search = false);

		virtual const Slot* FindItem(
			items::Item*, bool isSearched = false, bool searchFor = false);

		virtual void Popcorn(items::Item* item);
		virtual void Popcorn(items::Item* item, int stacks);
		virtual void Popcorn(items::Item* item, int stacks, int& stacksDropped);
		virtual void PopcornSlots(int slots);

		virtual void TakeSlot(Slot slot);
		virtual void TakeSlot(int index);

		void Close();

		void SelectSlot(Slot slot);
		void SelectSlot(int index);

		void DropAll();

		void TransferAll(items::Item* = nullptr, BaseInventory* tar = nullptr);
		void TransferAll(const std::string& term, BaseInventory* tar = nullptr);

		void Transfer(items::Item*, int amount = 1, BaseInventory* = nullptr,
			bool search = true);

		void SetFilter();
		void MakeNewFolder(std::string folderName);
		void AutoStack();
		void ToggleFolderView();

	private:
		void InitSlots(const window::Point& origin);
	};

}
