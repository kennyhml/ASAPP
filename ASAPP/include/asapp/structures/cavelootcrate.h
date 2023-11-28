#pragma once
#include "../interfaces/baseinventory.h"
#include "container.h"

namespace asa::structures
{
	class CaveLootCrate final : public Container
	{
	public:
		enum Quality
		{
			BLUE = 0,
			YELLOW = 1,
			RED = 2,
			ANY = BLUE | YELLOW | RED,
		};

		CaveLootCrate(Quality qualityFlags)
			: Container("Loot Crate"), qualityFlags(qualityFlags){};


		const Quality GetCrateQuality();

	private:
		Quality qualityFlags;
	};
}