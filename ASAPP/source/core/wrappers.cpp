#include "wrappers.h"
#include "../util/util.h"


namespace asa
{
	std::chrono::system_clock::time_point last_check;

	void asa::check_state()
	{
		if (!util::Timedout(last_check, std::chrono::seconds(1)) ||
			exceptions::get_crash_aware()) {
			return;
		}

		last_check = std::chrono::system_clock::now();

		if (interfaces::gMainMenu->GotConnectionTimeout()) {
			throw exceptions::ServerCrashedError();
		}

		if (window::HasCrashedPopup()) {
			throw exceptions::GameCrashedError();
		}
	}

	void asa::sleep_for(std::chrono::milliseconds duration)
	{
		check_state();
		return std::this_thread::sleep_for(duration);
	}

}
