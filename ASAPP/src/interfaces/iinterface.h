#pragma once

namespace asa::interfaces
{
	const int MAX_ITEMS_PER_PAGE = 36;

	class IInterface
	{
	public:
		virtual bool IsOpen() = 0;
	};
}
