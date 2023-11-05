#pragma once
#include <chrono>

namespace asa::interfaces
{
	using seconds = std::chrono::seconds;
	using ms = std::chrono::milliseconds;

	class IInterface
	{
	protected:
		virtual bool AwaitOpen(float timeout) = 0;

	public:
		virtual bool IsOpen() = 0;
	};
}
