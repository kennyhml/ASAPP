#include "dinoent.h"
#include "../_internal/util.h"

using namespace asa::entities;

void DinoEnt::Access()
{
	auto start = std::chrono::system_clock::now();
	while (!this->inventory->IsOpen()) {
		window::Press(settings::accessInventory, true);

		if (_internal::_util::Await(
				[this]() { return this->inventory->IsOpen(); },
				std::chrono::seconds(5))) {
			break;
		}
		if (_internal::_util::Timedout(start, std::chrono::seconds(30))) {
			throw std::runtime_error("Failed to access dino");
		}
	}

	while (this->inventory->IsReceivingRemoteInventory()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	};
}