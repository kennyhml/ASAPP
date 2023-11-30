#pragma once
#include "basetravelmap.h"
#include "components/searchbar.h"

namespace asa::interfaces
{
	class SpawnMap : public BaseTravelMap
	{
		using BaseTravelMap::BaseTravelMap;

	private:
		TravelSearchBar searchbar{ 93, 941, 240, 50 };

		components::Button newSurvivorButton{ 1210, 948, 244, 42 };
		components::Button regionsButton{ 176, 129, 176, 52 };
		components::Button bedsButton{ 351, 132, 129, 48 };

	public:
		bool IsOpen() override;

		void SpawnAt(int regionIndex = 0);
		void SpawnAt(const std::string& bed);

		virtual void GoTo(const std::string& destination);
	};

	inline SpawnMap* gSpawnMap = new SpawnMap();
}