#pragma once

namespace asa::interfaces
{
	class IInterface
	{
	public:
		virtual bool IsOpen() = 0;
	};
}
