#pragma once
#include "asapp/game/window.h"
#include "asapp/interfaces/iinterface.h"
#include "asapp/items/item.h"

namespace asa::interfaces
{
	class HUD : public IInterface
	{
	public:
		bool IsOpen() const { return true; }

		[[nodiscard]] bool IsMountOverweight() { return false; };
		[[nodiscard]] bool IsMountLow() { return false; };
		[[nodiscard]] bool IsMountOutOfFood() { return false; };
		[[nodiscard]] bool IsMountOutOfStamina() { return false; };
		[[nodiscard]] bool IsMountSprinting() { return false; };

		[[nodiscard]] bool IsPlayerOverweight();
		[[nodiscard]] bool IsPlayerBrokenBones();
		[[nodiscard]] bool IsPlayerOutOfWater();
		[[nodiscard]] bool IsPlayerOutOfFood();
		[[nodiscard]] bool IsPlayerSprinting();

		[[nodiscard]] bool CanDefaultTeleport();
		[[nodiscard]] bool CanFastTravel();
		[[nodiscard]] bool CanAccessInventory();

		[[nodiscard]] bool ExtendedInformationIsToggled();
		[[nodiscard]] bool GotItemAdded(items::Item*, window::Rect* roiOut);
		[[nodiscard]] bool GotItemRemoved(items::Item*, window::Rect* roiOut);
		[[nodiscard]] bool TransferredItemIntoDedicatedStorage();

		bool CountItemsAdded(items::Item&, int& amountOut);
		bool CountItemsRemoved(items::Item&, int& amountOut);

	private:
		window::Color blinkRedState{ 109, 54, 52 };
		window::Color blinkRedStateWeight{ 255, 45, 45 };

		window::Rect waterIcon{ 1868, 806, 34, 46 };
		window::Rect foodIcon{ 1861, 858, 46, 41 };
		window::Rect stamIcon{ 1872, 900, 29, 51 };
		window::Rect healthIcon{ 1861, 953, 48, 45 };
		window::Rect weightIcon{ 1860, 751, 51, 56 };
		window::Rect defaultTeleport{ 866, 911, 78, 27 };

		[[nodiscard]] bool IsBlinking(window::Rect icon, window::Color color,
			std::chrono::milliseconds maxDuration = std::chrono::milliseconds(
				400));

		[[nodiscard]] bool ItemRemoved(const window::Rect& area);
		[[nodiscard]] bool ItemAdded(const window::Rect& area);

		window::Rect itemIconRemovedOrAddedArea{ 7, 12, 41, 1067 };
		window::Rect itemRemovedArea{ 43, 20, 110, 1054 };
		window::Rect itemAddedArea{ 40, 15, 85, 1063 };

		window::Rect statusUpdateArea{ 844, 12, 224, 30 };
	};

	inline HUD* gHUD = new HUD();
}