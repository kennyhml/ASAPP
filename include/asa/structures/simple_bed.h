#pragma once
#include "interactable.h"
#include "asa/ui/maps/travelmap.h"

#include <utility>

namespace asa
{
    class simple_bed final : public interactable
    {
    public:
        explicit simple_bed(std::string name)
            : interactable(std::move(name), &get_action_mapping("Use"),
                           std::make_unique<travel_map>()) {}

        [[nodiscard]] travel_map* get_interface() const override
        {
            return dynamic_cast<travel_map*>(interface_.get());
        }
    };
}
