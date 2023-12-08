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

bool HUD::GotItemAdded(items::Item* item, window::Rect* roiOut)
{
	auto roi = item ? itemIconRemovedOrAddedArea : itemAddedArea;
	if (!item) {
		return ItemAdded(roi);
	}
	auto locations = window::LocateAllTemplate(
		roi, item->GetNotificationIcon(), 0.75, item->GetNotificationMask());

	for (const auto& rect : locations) {
		window::Rect matchRoi(roi.x + rect.x + 20, roi.y + rect.y, 120, 25);
		if (ItemAdded(matchRoi)) {
			if (roiOut) {
				*roiOut = matchRoi;
			}
			return true;
		}
	}
	return false;
}

bool HUD::GotItemRemoved(items::Item* item, window::Rect* roiOut)
{
	auto roi = item ? itemIconRemovedOrAddedArea : itemRemovedArea;
	if (!item) {
		return ItemRemoved(roi);
	}
	auto locations = window::LocateAllTemplate(
		roi, item->GetNotificationIcon(), 0.75, item->GetNotificationMask());

	for (const auto& rect : locations) {
		window::Rect matchRoi(roi.x + rect.x + 20, roi.y + rect.y, 120, 25);
		if (ItemRemoved(matchRoi)) {
			if (roiOut) {
				*roiOut = matchRoi;
			}
			return true;
		}
	}
	return false;
}

bool HUD::CountItemsAdded(items::Item& item, int& amountOut)
{
	window::Rect roi{ 0, 0, 0, 0 };
	if (!GotItemAdded(&item, &roi)) {
		return false;
	}

	roi = { roi.x + 85, roi.y, 100, roi.height };

	auto xLoc = window::LocateTemplate(roi, resources::text::x);
	if (!xLoc.has_value()) {
		std::cerr << "[!] Failed to locate x location." << std::endl;
		return false;
	}

	roi = { roi.x, roi.y, xLoc->x, roi.height };

	cv::Mat mask = window::GetMask(roi, window::Color(255, 255, 255), 15);
	window::SetTesseractImage(mask);
	window::tessEngine->SetPageSegMode(tesseract::PSM_SINGLE_WORD);
	window::tessEngine->SetVariable("tessedit_char_whitelist", "0123456789");

	std::string resultString = window::tessEngine->GetUTF8Text();
	if (resultString.empty() || resultString == "\\n") {
		std::cerr << "[!] OCR failed, no result determined." << std::endl;
		return false;
	}

	amountOut = std::stoi(resultString);
	return true;
}

bool HUD::CountItemsRemoved(items::Item& item, int& amountOut)
{
	window::Rect roi{ 0, 0, 0, 0 };
	if (!GotItemRemoved(&item, &roi)) {
		return false;
	}

	roi = { roi.x + 110, roi.y, 100, roi.height };

	auto xLoc = window::LocateTemplate(roi, resources::text::x);
	if (!xLoc.has_value()) {
		std::cerr << "[!] Failed to locate x location." << std::endl;
		return false;
	}

	roi = { roi.x, roi.y, xLoc->x, roi.height };
	return false;
}