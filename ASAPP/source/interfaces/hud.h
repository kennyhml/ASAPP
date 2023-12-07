#pragma once
#include "asapp/game/window.h"
#include "asapp/interfaces/iinterface.h"

namespace asa::interfaces
{
	class HUD : public IInterface
	{

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

		window::Rect invClosedItemAddedOrRemovedArea{ 0, 260, 200, 615 };
		window::Rect invOpenItemAddedOrRemovedArea{ 0, 765, 200, 315 };
		window::Rect statusUpdateArea{ 844, 12, 224, 30 };

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
		[[nodiscard]] bool GotItemAdded(bool isInventoryOpen);
		[[nodiscard]] bool GotItemRemoved(bool isInventoryOpen);
		[[nodiscard]] bool TransferredItemIntoDedicatedStorage();
	};

	inline HUD* gHUD = new HUD();
}