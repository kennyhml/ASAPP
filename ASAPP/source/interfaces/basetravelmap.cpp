#include "asapp/interfaces/basetravelmap.h"
#include "../core/wrappers.h"
#include "../util/util.h"
#include "asapp/game/globals.h"
#include "asapp/game/resources.h"

const bool asa::interfaces::BaseTravelMap::TravelSearchBar::HasTextEntered()
{
	window::Color textColor(134, 234, 255);

	auto mask = window::GetMask(this->area, textColor, 30);
	return cv::countNonZero(mask) > 100;
}

const bool
asa::interfaces::BaseTravelMap::TravelSearchBar::HasBlinkingCursor() const
{
	window::Color textColor(134, 234, 255);

	auto mask = window::GetMask(this->area, textColor, 30);
	return cv::countNonZero(mask) > 20;
}

void asa::interfaces::BaseTravelMap::TravelSearchBar::SearchFor(
	std::string term)
{
	this->Press();
	SleepFor(std::chrono::milliseconds(200));
	this->isSearching = true;

	if (!globals::useWindowInput) {
		util::SetClipboard(term);
		controls::KeyCombinationPress("ctrl", "v");
	}
	else {
		for (auto c : term) {
			if (globals::useWindowInput) {
				window::PostChar(c);
			}
		}
	}

	if (!util::Await([this]() { return this->HasTextEntered(); },
			std::chrono::seconds(5))) {
		std::cerr << "[!] Failed to search, trying again..." << std::endl;
		return this->SearchFor(term);
	}

	SleepFor(std::chrono::milliseconds(50));
	window::PostKeyPress("Esc");

	this->isSearching = false;
	this->lastSearchedTerm = term;
	this->isTextEntered = true;
}

void asa::interfaces::BaseTravelMap::TravelSearchBar::Press() const
{
	window::Point loc = this->area.GetRandLocation(8);

	do {
		window::ClickAt(loc, controls::LEFT);


	} while (!util::Await([this]() { return this->HasBlinkingCursor(); },
		std::chrono::milliseconds(500)));
}

const bool asa::interfaces::BaseTravelMap::IsOpen() const
{
	return window::MatchTemplate(this->dayTime, resources::interfaces::day);
}

bool asa::interfaces::BaseTravelMap::CanConfirmTarget()
{
	static window::Color readyColor(158, 88, 18);

	auto mask = window::GetMask(confirmButton.area, readyColor, 20);
	return cv::countNonZero(mask) > 50;
}

bool asa::interfaces::BaseTravelMap::HasResult()
{
	static window::Color selectedColor(255, 255, 255);
	static window::Color textColor(133, 226, 243);

	for (const components::Button& result : results) {
		auto mask = window::GetMask(result.area, selectedColor, 20);
		auto mask2 = window::GetMask(result.area, textColor, 20);

		if (cv::countNonZero(mask) > 30 || cv::countNonZero(mask2) > 30) {
			return true;
		};
	}

	return false;
}

bool asa::interfaces::BaseTravelMap::IsResultSelected(int index)
{
	static window::Color hoveredSelectedColor(83, 39, 1);
	static window::Color selectedColor(128, 64, 2);

	auto roi = this->results[index].area;

	auto mask1 = window::GetMask(roi, hoveredSelectedColor, 30);
	auto mask2 = window::GetMask(roi, selectedColor, 30);

	return cv::countNonZero(mask1) > 50 || cv::countNonZero(mask2) > 50;
};

int asa::interfaces::BaseTravelMap::CountResults()
{
	static window::Color selectedColor(255, 255, 255);
	static window::Color textColor(133, 226, 243);

	int count = 0;
	for (const components::Button& result : results) {
		auto mask = window::GetMask(result.area, selectedColor, 15);
		auto mask2 = window::GetMask(result.area, textColor, 15);

		count += (cv::countNonZero(mask) > 50 || cv::countNonZero(mask2) > 50);
	}

	return count;
}

void asa::interfaces::BaseTravelMap::SelectResult(int index)
{
	do {
		this->results[index].Press();
	} while (
		!util::Await([this, index]() { return this->IsResultSelected(index); },
			std::chrono::seconds(3)));
}
