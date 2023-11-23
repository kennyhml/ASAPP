#pragma once
#include "../game/window.h"
#include "iinterface.h"

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

		[[nodiscard]] const bool IsBlinking(window::Rect icon,
			window::Color color,
			std::chrono::milliseconds maxDuration = std::chrono::milliseconds(
				400));

	public:
		bool IsOpen() { return true; }

		[[nodiscard]] const bool IsMountOverweight() { return false; };
		[[nodiscard]] const bool IsMountLow() { return false; };
		[[nodiscard]] const bool IsMountOutOfFood() { return false; };
		[[nodiscard]] const bool IsMountOutOfStamina() { return false; };
		[[nodiscard]] const bool IsMountSprinting() { return false; };

		[[nodiscard]] const bool IsPlayerOverweight();
		[[nodiscard]] const bool IsPlayerBrokenBones();
		[[nodiscard]] const bool IsPlayerOutOfWater();
		[[nodiscard]] const bool IsPlayerOutOfFood();
		[[nodiscard]] const bool IsPlayerSprinting();
	};

	inline HUD* gHUD = new HUD();
}