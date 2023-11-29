#include "asapp/interfaces/components/searchbar.h"
#include "../../util/util.h"
#include "asapp/game/controls.h"
#include "asapp/game/globals.h"

void asa::interfaces::components::SearchBar::Press() const
{
	window::Point loc = this->area.GetRandLocation(8);
	for (int i = 0; i < 2; i++) {
		window::ClickAt(loc, controls::LEFT);
	}
}

void asa::interfaces::components::SearchBar::SearchFor(std::string term)
{
	this->Press();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
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

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
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