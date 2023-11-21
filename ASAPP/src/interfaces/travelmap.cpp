#include "travelmap.h"
#include "../_internal/util.h"
#include "exceptions.h"

void asa::interfaces::TravelMap::Close()
{
	auto start = std::chrono::system_clock::now();
	while (this->IsOpen()) {
		window::Press("esc", true);
		if (_internal::_util::Await([this]() { return !this->IsOpen(); },
				std::chrono::seconds(5))) {
			return;
		}

		if (_internal::_util::Timedout(start, std::chrono::seconds(30))) {
			throw exceptions::InterfaceNotClosedError(this);
		}
	}
}

void asa::interfaces::TravelMap::GoTo(std::string destination)
{
	std::cout << "[+] Traveling to '" << destination << "'..." << std::endl;
	this->searchbar.SearchFor(destination);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	this->SelectResult();

	std::cout << "\t[-] Waiting for fast travel to go off cooldown...";
	while (!this->CanConfirmTarget()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	std::cout << " Done." << std::endl;

	while (this->IsOpen()) {
		this->confirmButton.Press();
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	std::cout << "\t[-] Traveled to '" << destination << "'." << std::endl;

	this->searchbar.SetTextCleared();
}
