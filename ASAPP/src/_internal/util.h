#pragma once
#include <chrono>
#include <functional>

namespace internal::_util
{
	bool Await(const std::function<bool()>& condition,
		std::chrono::milliseconds timeout);

	template <typename Type> const char* GetName(const Type& obj)
	{
		return typeid(obj).name();
	}



}
