#pragma once
#include "components/button.h"
#include "iinterface.h"

namespace asa::interfaces
{
	class MainMenu : public IInterface
	{
	private:
		components::Button esc{ 1749, 88, 46, 34 };

	public:
		const bool IsOpen() const override;
	};
}