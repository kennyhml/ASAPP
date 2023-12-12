#pragma once

namespace asa::interfaces
{
	class IInterface
	{
	public:
		[[nodiscard]] virtual bool is_open() const = 0;
	};
}
