#include "hud.h"
#include "../util/util.h"
#include "asapp/game/resources.h"
#include "asapp/game/window.h"

using namespace asa::interfaces;

bool HUD::IsBlinking(window::Rect icon, window::Color color,
	std::chrono::milliseconds maxDuration)
{
	auto start = std::chrono::system_clock::now();
	while (!util::Timedout(start, maxDuration)) {
		auto masked = window::GetMask(icon, color, 30);
		if (cv::countNonZero(masked) > 20) {
			return true;
		}
	}
	return false;
}

bool HUD::ItemRemoved(const window::Rect& area)
{
	return window::MatchTemplate(area, resources::text::removed);
}

bool HUD::ItemAdded(const window::Rect& area)
{
	return window::MatchTemplate(area, resources::text::added);
}

bool HUD::IsPlayerOverweight()
{
	return this->IsBlinking(this->weightIcon, this->blinkRedStateWeight,
		std::chrono::milliseconds(700));
}

bool HUD::IsPlayerBrokenBones()
{
	return this->IsBlinking(this->healthIcon, this->blinkRedState);
}

bool HUD::IsPlayerOutOfWater()
{
	return this->IsBlinking(this->waterIcon, this->blinkRedState);
}

bool HUD::IsPlayerOutOfFood()
{
	return this->IsBlinking(this->foodIcon, this->blinkRedState);
}

bool HUD::IsPlayerSprinting() { return false; }

bool HUD::CanDefaultTeleport()
{
	return window::MatchTemplate(
		this->defaultTeleport, resources::text::default_teleport, 0.5);
}

bool HUD::CanFastTravel()
{
	return window::MatchTemplate(
		window::Screenshot(), resources::text::fast_travel);
}

bool HUD::CanAccessInventory()
{
	return window::MatchTemplate(
		window::Screenshot(), resources::text::access_inventory);
}

bool HUD::ExtendedInformationIsToggled()
{
	static window::Rect roi{ 14, 34, 134, 35 };
	return window::MatchTemplate(roi, resources::text::day);
}

bool HUD::GotItemAdded(bool isInventoryOpen, items::Item* item)
{
	auto roi = isInventoryOpen ? invOpenItemAddedOrRemovedArea
							   : invClosedItemAddedOrRemovedArea;
	if (item) {
		auto loc = window::LocateTemplate(
			roi, item->GetNotificationIcon(), 0.7, item->GetNotificationMask());
		if (!loc.has_value()) {
			return false;
		}
		roi = { roi.x + loc->x + 20, roi.y + loc->y, 120, 25 };
	}
	return ItemAdded(roi);
}

bool HUD::GotItemRemoved(bool isInventoryOpen, items::Item* item)
{
	auto roi = isInventoryOpen ? invOpenItemAddedOrRemovedArea
							   : invClosedItemAddedOrRemovedArea;
	if (item) {
		auto loc = window::LocateTemplate(
			roi, item->GetNotificationIcon(), 0.7, item->GetNotificationMask());
		if (!loc.has_value()) {
			return false;
		}
		roi = { roi.x + loc->x + 20, roi.y + loc->y, 120, 25 };
	}
	return ItemRemoved(roi);
}