#pragma once
#include "interactablestructure.h"
#include "../interfaces/teleportmap.h"

namespace asa::structures
{
    class Teleporter : public InteractableStructure
    {
    public:
        Teleporter(std::string name) : InteractableStructure(
            name, &settings::use, new interfaces::TeleportMap())
        {
            this->map = static_cast<interfaces::TeleportMap*>(_interface);
        };

        interfaces::TeleportMap* map;
    };
}
