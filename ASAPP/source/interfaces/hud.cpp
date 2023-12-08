#include "hud.h"
#include "../util/util.h"
#include "asapp/game/resources.h"
#include "asapp/game/window.h"
#include <stdexcept>

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

bool HUD::GotItemAdded(
	bool isInventoryOpen, items::Item* item, window::Rect* roiOut)
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

	if (roiOut) {
		*roiOut = roi;
	}
	return ItemAdded(roi);
}

bool HUD::GotItemRemoved(
	bool isInventoryOpen, items::Item* item, window::Rect* roiOut)
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
	if (roiOut) {
		*roiOut = roi;
	}
	return ItemRemoved(roi);
}

bool HUD::CountItemsAdded(bool invOpen, items::Item& item, int& amountOut)
{
	window::Rect roi{ 0, 0, 0, 0 };
	if (!GotItemAdded(invOpen, &item, &roi)) {
		return false;
	}

	roi = { roi.x + 85, roi.y, 100, roi.height };

	auto xLoc = window::LocateTemplate(roi, resources::text::x);
	if (!xLoc.has_value()) {
		std::cerr << "[!] Failed to locate x location." << std::endl;
		return false;
	}

	roi = { roi.x, roi.y, xLoc->x, roi.height };
	cv::imshow("t", window::Screenshot(roi));
	cv::waitKey(0);
	return false;
}


bool HUD::CountItemsRemoved(bool invOpen, items::Item& item, int& amountOut)
{
	window::Rect roi{ 0, 0, 0, 0 };
	if (!GotItemRemoved(invOpen, &item, &roi)) {
		return false;
	}

	roi = { roi.x + 110, roi.y, 100, roi.height };

	auto xLoc = window::LocateTemplate(roi, resources::text::x);
	if (!xLoc.has_value()) {
		std::cerr << "[!] Failed to locate x location." << std::endl;
		return false;
	}

	roi = { roi.x, roi.y, xLoc->x, roi.height };
	cv::imshow("t", window::Screenshot(roi));
	cv::waitKey(0);
	return false;
}