#include "asapp/interfaces/travelmap.h"
#include "../core/wrappers.h"
#include "../util/util.h"
#include "asapp/interfaces/exceptions.h"

void asa::interfaces::TravelMap::Close()
{
	auto start = std::chrono::system_clock::now();
	while (this->IsOpen()) {
		window::Press("esc", true);
		if (util::Await([this]() { return !this->IsOpen(); },
				std::chrono::seconds(5))) {
			return;
		}

		if (util::Timedout(start, std::chrono::seconds(30))) {
			throw exceptions::InterfaceNotClosedError(this);
		}
	}
}

void asa::interfaces::TravelMap::GoTo(const std::string& destination)
{
	std::cout << "[+] Traveling to '" << destination << "'..." << std::endl;
	this->searchbar.SearchFor(destination);
	SleepFor(std::chrono::milliseconds(300));

	this->SelectResult();

	std::cout << "\t[-] Waiting for fast travel to go off cooldown...";
	while (!this->CanConfirmTarget()) {
		SleepFor(std::chrono::milliseconds(50));
	}
	std::cout << " Done." << std::endl;

	while (this->IsOpen()) {
		this->confirmButton.Press();
		SleepFor(std::chrono::milliseconds(200));
	}
	std::cout << "\t[-] Traveled to '" << destination << "'." << std::endl;

	this->searchbar.SetTextCleared();
}
