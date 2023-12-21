#pragma once
#include <utility>

#include "interactablestructure.h"
#include "../interfaces/travelmap.h"

namespace asa::structures
{
    class SimpleBed : public InteractableStructure
    {
    public:
        explicit SimpleBed(std::string name) : InteractableStructure(
            std::move(name), &settings::use, new interfaces::TravelMap())
        {
            this->map = reinterpret_cast<interfaces::TravelMap*>(_interface);
        };

        interfaces::TravelMap* map;
    };
}
