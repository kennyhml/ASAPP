#pragma once
#include <chrono>
#include <functional>

namespace internal::_util
{
	bool Await(const std::function<bool()>& condition,
		std::chrono::milliseconds timeout);

	bool Timedout(std::chrono::system_clock::time_point& start,
		std::chrono::seconds timeout);

	template <typename Type> const char* GetName(const Type& obj)
	{
		return typeid(obj).name();
	}



}
