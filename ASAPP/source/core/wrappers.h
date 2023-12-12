#pragma once
#include "asapp/core/exceptions.h"
#include "asapp/interfaces/mainmenu.h"
#include <chrono>

namespace asa
{
	void check_state();
	void sleep_for(std::chrono::milliseconds duration);
}