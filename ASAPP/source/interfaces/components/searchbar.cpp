#include "asapp/interfaces/components/searchbar.h"
#include "../../core/wrappers.h"
#include "../../util/util.h"
#include "asapp/game/controls.h"
#include "asapp/game/globals.h"

using namespace asa::interfaces::components;


bool SearchBar::HasTextEntered()
{
	window::Color textColor(134, 234, 255);

	auto mask = window::GetMask(this->area, textColor, 30);
	return cv::countNonZero(mask) > 50;
}

bool SearchBar::HasBlinkingCursor() const
{
	window::Color textColor(134, 234, 255);

	auto mask = window::GetMask(this->area, textColor, 30);
	return cv::countNonZero(mask) > 15;
}

void SearchBar::SearchFor(std::string term)
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
	window::Press("enter");

	this->isSearching = false;
	this->lastSearchedTerm = term;
	this->isTextEntered = true;
}

void SearchBar::Press() const
{
	window::Point loc = this->area.GetRandLocation(8);

	do {
		window::PostMousePressAt(loc, controls::LEFT);
	} while (!util::Await([this]() { return this->HasBlinkingCursor(); },
		std::chrono::milliseconds(500)));
}

void asa::interfaces::components::SearchBar::DeleteSearch()
{
	this->Press();

	if (globals::useWindowInput) {
		for (int i = 0; i < lastSearchedTerm.size(); i++) {
			window::PostKeyPress("BackSpace", false);
			window::PostKeyPress("Delete", false);
		}
	}
	else {
		controls::KeyCombinationPress("Ctrl", "a");
		SleepFor(std::chrono::milliseconds(40));
		controls::KeyPress("Delete");
	}

	SleepFor(std::chrono::milliseconds(50));
	window::Press("enter");

	this->SetTextCleared();
}