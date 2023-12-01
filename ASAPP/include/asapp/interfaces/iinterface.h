#pragma once

namespace asa::interfaces
{
	class IInterface
	{
	public:
		[[nodiscard]] virtual const bool IsOpen() const = 0;
	};
}
