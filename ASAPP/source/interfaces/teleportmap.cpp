#include "asapp/interfaces/teleportmap.h"
#include "../core/wrappers.h"
#include "../util/util.h"
#include "asapp/interfaces/exceptions.h"

namespace asa::interfaces
{
	void asa::interfaces::TeleportMap::close()
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

	void asa::interfaces::TeleportMap::set_selected_as_default()
	{
		while (!can_confirm_target()) {
			sleep_for(std::chrono::milliseconds(50));
		}

		set_default_button.press();
	}

	void asa::interfaces::TeleportMap::go_to(const std::string& destination)
	{
		std::cout << "[+] Teleporting to '" << destination << "'..."
				  << std::endl;
		searchbar.search_for(destination);
		sleep_for(std::chrono::milliseconds(400));

		select_result();

		std::cout << "\t[-] Waiting for teleport to go off cooldown...";
		while (!can_confirm_target()) {
			sleep_for(std::chrono::milliseconds(50));
		}
		std::cout << " Done." << std::endl;

		while (is_open()) {
			confirm_button.press();
			sleep_for(std::chrono::milliseconds(200));
		}
		std::cout << "\t[-] Teleported to '" << destination << "'."
				  << std::endl;
		searchbar.set_text_cleared();
	}



}
