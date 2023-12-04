#include "wrappers.h"
#include "../util/util.h"

void asa::CheckState()
{
	if (!util::Timedout(lastCheck, std::chrono::seconds(1))) {
		return;
	}
	lastCheck = std::chrono::system_clock::now();

	if (interfaces::gMainMenu->GotConnectionTimeout()) {
		throw exceptions::ServerCrashedError();
	}

	if (window::HasCrashedPopup()) {
		throw exceptions::GameCrashedError();
	}
}

void asa::SleepFor(std::chrono::milliseconds duration)
{
	CheckState();
	return std::this_thread::sleep_for(duration);
}