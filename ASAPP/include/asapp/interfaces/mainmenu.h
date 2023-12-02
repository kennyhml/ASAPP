#pragma once
#include "components/button.h"
#include "iinterface.h"

namespace asa::interfaces
{
	class MainMenu : public IInterface
	{
	private:
		components::Button esc{ 1749, 88, 46, 34 };
		components::Button accept{ 764, 710, 376, 42 };

	public:
		const bool IsOpen() const override;
		const bool GotConnectionTimeout() const;
	};

	inline MainMenu* gMainMenu = new MainMenu();

}