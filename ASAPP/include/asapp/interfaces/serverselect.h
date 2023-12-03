#pragma once
#include "components/button.h"
#include "components/searchbar.h"

#include "iinterface.h"

namespace asa::interfaces
{

	class ServerSelect final : public IInterface
	{
	private:
		components::SearchBar searchbar{ 823, 924, 273, 51 };
		components::Button officialButton{ 571, 182, 116, 49 };
		components::Button refreshButton{ 834, 932, 275, 39 };

		components::Button joinButton{ 1615, 924, 195, 41 };

	public:
		bool IsOpen() const;

		void JoinServer(const std::string& serverName);
		void Refresh();

		bool CanJoin();
		bool CanJoinLastPlayed();
	};





}