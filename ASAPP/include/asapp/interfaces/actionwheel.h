#pragma once

#include "../game/window.h"
#include "iinterface.h"

namespace asa::interfaces
{
	class ActionWheel : public IInterface
	{
	private:
		window::Rect nameArea{ 815, 461, 294, 160 };
		window::Rect area{ 683, 253, 543, 556 };

	public:
		const bool IsOpen() const override;
		void SelectLayOn() const;
	};
}
