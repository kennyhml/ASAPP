#pragma once
#include "components/button.h"
#include "components/searchbar.h"

#include "iinterface.h"

namespace asa::interfaces
{

	class ServerSelect final : public IInterface
	{
	private:
		components::SearchBar searchbar{ 1577, 172, 184, 46 };

		window::Rect joiningText{ 717, 469, 507, 38 };
		window::Rect joiningText2{ 828, 357, 266, 66 };

		components::Button officialButton{ 571, 182, 116, 49 };
		components::Button refreshButton{ 834, 932, 275, 39 };
		components::Button bestResult{ 94, 306, 1730, 43 };
		components::Button joinButton{ 1615, 924, 195, 41 };
		components::Button joinLastPlayedButton{ 1624, 876, 178, 39 };

		bool IsBestResultSelected();

	public:
		bool IsOpen() const;
		bool CanJoin() const;
		bool CanJoinLastPlayed() const;
		bool IsJoiningServer() const;

		void JoinServer(const std::string& serverName);
		void Refresh();
	};

	inline ServerSelect* gServerSelect = new ServerSelect();
}