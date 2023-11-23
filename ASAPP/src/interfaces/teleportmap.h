#pragma once
#include "basetravelmap.h"
#include "components/searchbar.h"

namespace asa::interfaces
{
	class TeleportMap : public BaseTravelMap
	{
		using BaseTravelMap::BaseTravelMap;

	private:
		components::SearchBar searchbar{ 301, 940, 245, 51 };
		components::Button setDefaultButton{ 1210, 948, 245, 43 };

	public:
		void Close();
		void SetSelectedAsDefault();

		virtual void GoTo(const std::string& destination);
	};

	inline TeleportMap* gTeleportMap = new TeleportMap();


}