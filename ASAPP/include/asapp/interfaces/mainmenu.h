#pragma once
#include "components/button.h"
#include "iinterface.h"

namespace asa::interfaces
{
	class MainMenu final : public IInterface
	{
	private:
		components::Button esc{ 1732, 87, 82, 34 };
		components::Button accept{ 764, 710, 376, 42 };
		components::Button joinLastSession{ 823, 924, 273, 51 };
		components::Button startButton{ 772, 826, 377, 70 };

	public:
		bool IsOpen() const override;
		bool GotConnectionTimeout() const;

		void Start();
		void JoinLastSession();
	};

	inline MainMenu* gMainMenu = new MainMenu();

}