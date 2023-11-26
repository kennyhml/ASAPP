#pragma once
#include "../interfaces/travelmap.h"
#include "interactablestructure.h"

namespace asa::structures
{
	class SimpleBed : public InteractableStructure
	{
	public:
		SimpleBed(std::string name)
			: InteractableStructure(
				  name, &settings::use, new interfaces::TravelMap())
		{
			this->map = static_cast<interfaces::TravelMap*>(_interface);
		};

		interfaces::TravelMap* map;
	};

}