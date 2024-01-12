#pragma once
#include "interactablestructure.h"
#include "../interfaces/teleportmap.h"

namespace asa::structures
{
    class Teleporter : public InteractableStructure
    {
    public:
        explicit Teleporter(std::string name) : InteractableStructure(
            std::move(name), &settings::use,
            std::make_unique<interfaces::TeleportMap>()) {}

        [[nodiscard]] interfaces::TeleportMap* get_interface() const override
        {
            return dynamic_cast<interfaces::TeleportMap*>(interface_.get());
        }
    };
}
