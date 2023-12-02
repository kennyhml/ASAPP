#pragma once
#include "asapp/core/exceptions.h"
#include "asapp/interfaces/mainmenu.h"
#include <chrono>

namespace asa
{
	inline std::chrono::system_clock::time_point lastCheck;

	void CheckState();
	void SleepFor(std::chrono::milliseconds duration);
}