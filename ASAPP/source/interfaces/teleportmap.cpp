#include "asapp/interfaces/teleportmap.h"
#include "../util/util.h"
#include "asapp/interfaces/exceptions.h"

void asa::interfaces::TeleportMap::Close()
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

void asa::interfaces::TeleportMap::SetSelectedAsDefault()
{
	while (!this->CanConfirmTarget()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	this->setDefaultButton.Press();
}

void asa::interfaces::TeleportMap::GoTo(const std::string& destination)
{
	std::cout << "[+] Teleporting to '" << destination << "'..." << std::endl;
	this->searchbar.SearchFor(destination);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	this->SelectResult();

	std::cout << "\t[-] Waiting for teleport to go off cooldown...";
	while (!this->CanConfirmTarget()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	std::cout << " Done." << std::endl;

	while (this->IsOpen()) {
		this->confirmButton.Press();
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	std::cout << "\t[-] Teleported to '" << destination << "'." << std::endl;
	this->searchbar.SetTextCleared();
}
