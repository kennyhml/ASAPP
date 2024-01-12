#pragma once
#include <utility>

#include "interactablestructure.h"
#include "../interfaces/travelmap.h"

namespace asa::structures
{
    class SimpleBed final : public InteractableStructure
    {
    public:
        explicit SimpleBed(std::string name) : InteractableStructure(
            std::move(name), &settings::use, std::make_unique<interfaces::TravelMap>()) {}

        [[nodiscard]] interfaces::TravelMap* get_interface() const override
        {
            return dynamic_cast<interfaces::TravelMap*>(interface_.get());
        }
    };
}
