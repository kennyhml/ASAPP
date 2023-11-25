#include "hud.h"
#include "../_internal/util.h"
#include "../game/resources.h"
#include "../game/window.h"

using namespace asa::interfaces;

const bool HUD::IsBlinking(window::Rect icon, window::Color color,
	std::chrono::milliseconds maxDuration)
{
	auto start = std::chrono::system_clock::now();
	while (!_internal::_util::Timedout(start, maxDuration)) {
		auto masked = window::GetMask(icon, color, 30);
		if (cv::countNonZero(masked) > 20) {
			return true;
		}
	}
	return false;
}

const bool HUD::IsPlayerOverweight()
{
	return this->IsBlinking(this->weightIcon, this->blinkRedStateWeight,
		std::chrono::milliseconds(700));
}

const bool HUD::IsPlayerBrokenBones()
{
	return this->IsBlinking(this->healthIcon, this->blinkRedState);
}

const bool HUD::IsPlayerOutOfWater()
{
	return this->IsBlinking(this->waterIcon, this->blinkRedState);
}

const bool HUD::IsPlayerOutOfFood()
{
	return this->IsBlinking(this->foodIcon, this->blinkRedState);
}

const bool HUD::IsPlayerSprinting() { return false; }

const bool HUD::CanDefaultTeleport()
{
	window::Color white(255, 255, 255);
	auto mask = window::GetMask(this->defaultTeleport, white, 30);
	return cv::countNonZero(mask) > 50;
}

const bool HUD::ExtendedInformationIsToggled()
{
	static window::Rect roi{ 14, 34, 134, 35 };
	return window::MatchTemplate(roi, resources::day);
}

const bool HUD::GotItemAdded(bool isInventoryOpen)
{
	auto roi = isInventoryOpen ? this->invOpenItemAddedOrRemovedArea
							   : this->invClosedItemAddedOrRemovedArea;

	return window::MatchTemplate(roi, resources::text::added);
}

const bool HUD::GotItemRemoved(bool isInventoryOpen)
{
	auto roi = isInventoryOpen ? this->invOpenItemAddedOrRemovedArea
							   : this->invClosedItemAddedOrRemovedArea;

	return window::MatchTemplate(roi, resources::text::removed);
}