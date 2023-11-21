#include "searchbar.h"
#include "../../game/controls.h"
#include "../../game/globals.h"
#include <Windows.h>

void asa::interfaces::components::SearchBar::Press() const
{
	window::Point loc = this->area.GetRandLocation(5);
	window::ClickAt(loc, controls::LEFT);
}

void asa::interfaces::components::SearchBar::SearchFor(std::string term)
{
	this->Press();
	this->isSearching = true;

	for (auto c : term) {
		if (globals::useWindowInput) {
			window::PostChar(c);
		}
		else {
			controls::KeyPress(std::string(1, c));
		}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	window::PostKeyPress("Esc");

	this->isSearching = false;
	this->lastSearchedTerm = term;
	this->isTextEntered = true;
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
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
		controls::KeyPress("Delete");
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	window::Press("Esc");

	this->SetTextCleared();
}