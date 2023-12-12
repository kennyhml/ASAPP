#pragma once
#include "basetravelmap.h"
#include "components/searchbar.h"

namespace asa::interfaces
{
	class TeleportMap : public BaseTravelMap
	{
		using BaseTravelMap::BaseTravelMap;

	private:
		components::Button set_default_button{ 1210, 948, 245, 43 };

	public:
		components::SearchBar searchbar{ 307, 946, 226, 43 };
		void close();
		void set_selected_as_default();

		void go_to(const std::string& destination);
	};

	inline TeleportMap* gTeleportMap = new TeleportMap();
}