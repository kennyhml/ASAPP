#pragma once

#include "../game/window.h"
#include "iinterface.h"

namespace asa::interfaces
{
	class ActionWheel : public IInterface
	{
	private:
		window::Rect nameArea{ 815, 461, 294, 160 };

	public:
		bool IsOpen();
	};
}
