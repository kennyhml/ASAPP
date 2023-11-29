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
			BLUE = 1,
			YELLOW = 2,
			RED = 4,
			ANY = BLUE | YELLOW | RED,
		};

		CaveLootCrate(int qualityFlags)
			: Container("Loot Crate"), qualityFlags(qualityFlags){};

		const Quality GetCrateQuality();

	private:
		std::optional<asa::window::Rect> GetInfoArea();
		const Quality GetQualityFromTooltip(std::string tooltip);
		int qualityFlags;
	};
}