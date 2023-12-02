#pragma once
#include <chrono>

namespace asa
{

	void Sleep(std::chrono::milliseconds duration)
	{
		return std::this_thread::sleep_for(duration);
	}

}