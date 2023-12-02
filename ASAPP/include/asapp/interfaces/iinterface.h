#pragma once

namespace asa::interfaces
{
	class IInterface
	{
	public:
		[[nodiscard]] virtual bool IsOpen() const = 0;
	};
}
