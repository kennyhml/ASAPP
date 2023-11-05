#include "util.h"

bool internal::_util::Await(
	const std::function<bool()>& condition, std::chrono::milliseconds timeout)
{
	auto start_time = std::chrono::steady_clock::now();
	while (!condition()) {
		auto current_time = std::chrono::steady_clock::now();
		auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(
			current_time - start_time);

		if (elapsed_time >= timeout) {
			return false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
	return true;
}

bool internal::_util::Timedout(
	std::chrono::system_clock::time_point& start, std::chrono::seconds timeout)
{
	auto now = std::chrono::system_clock::now();
	auto timePassed = now - start;

	return timePassed >= timeout;
}
