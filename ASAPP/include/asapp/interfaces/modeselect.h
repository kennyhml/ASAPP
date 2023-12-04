#pragma once
#include "components/button.h"
#include "iinterface.h"

namespace asa::interfaces
{
	class ModeSelect final : public IInterface
	{
	private:
		components::Button joinGameButton{ 540, 247, 383, 640 };
		components::Button createOrResumeGameButton{ 997, 251, 382, 634 };
		components::Button modsListButton{ 1449, 247, 396, 647 };
		components::Button backButton{ 893, 946, 135, 36 };

	public:
		bool IsOpen() const;

		void JoinGame();
	};

	inline ModeSelect* gModeSelect = new ModeSelect();
}