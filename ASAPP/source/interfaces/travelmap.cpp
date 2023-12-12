#include "asapp/interfaces/travelmap.h"
#include "../core/wrappers.h"
#include "../util/util.h"
#include "asapp/interfaces/exceptions.h"

namespace asa::interfaces
{
	void TravelMap::close()
	{
		auto start = std::chrono::system_clock::now();
		while (is_open()) {
			window::press("esc", true);
			if (util::await(
					[this]() { return !is_open(); }, std::chrono::seconds(5))) {
				return;
			}

			if (util::timedout(start, std::chrono::seconds(30))) {
				throw InterfaceNotClosedError(this);
			}
		}
	}

	void TravelMap::go_to(const std::string& destination)
	{
		std::cout << "[+] Traveling to '" << destination << "'..." << std::endl;
		searchbar.search_for(destination);
		sleep_for(std::chrono::milliseconds(300));

		select_result();

		std::cout << "\t[-] Waiting for fast travel to go off cooldown...";
		while (!can_confirm_target()) {
			sleep_for(std::chrono::milliseconds(50));
		}
		std::cout << " Done." << std::endl;

		while (is_open()) {
			confirm_button.press();
			sleep_for(std::chrono::milliseconds(500));
		}
		std::cout << "\t[-] Traveled to '" << destination << "'." << std::endl;

		searchbar.set_text_cleared();
	}


}
