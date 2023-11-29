#pragma once
#include "../interfaces/teleportmap.h"
#include "interactablestructure.h"

namespace asa::structures
{
	class Teleporter : public InteractableStructure
	{
	public:
		Teleporter(std::string name)
			: InteractableStructure(
				  name, &settings::use, new interfaces::TeleportMap())
		{
			this->map = static_cast<interfaces::TeleportMap*>(_interface);
		};

		interfaces::TeleportMap* map;
	};

}