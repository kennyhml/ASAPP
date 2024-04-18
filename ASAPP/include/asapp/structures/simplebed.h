#pragma once
#include <utility>

#include "interactable.h"
#include "../interfaces/travelmap.h"

namespace asa::structures
{
    class SimpleBed final : public Interactable
    {
    public:
        explicit SimpleBed(std::string name) : Interactable(
            std::move(name), &settings::use, std::make_unique<interfaces::TravelMap>()) {}

        [[nodiscard]] interfaces::TravelMap* get_interface() const override
        {
            return dynamic_cast<interfaces::TravelMap*>(interface_.get());
        }
    };
}
