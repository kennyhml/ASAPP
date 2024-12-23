#pragma once
#include "interactable.h"
#include "asa/interfaces/maps/teleport_map.h"

namespace asa
{
    class teleporter final : public interactable
    {
    public:
        enum Size : int32_t
        {
            SMALL,
            MEDIUM,
            LARGE
        };

        explicit teleporter(std::string t_name, const Size t_size = SMALL)
            : interactable(std::move(t_name), &get_action_mapping("Use"),
                           std::make_unique<teleport_map>()), size_(t_size) {}

        [[nodiscard]] teleport_map* get_interface() const override
        {
            return dynamic_cast<teleport_map*>(interface_.get());
        }

    private:
        Size size_;
    };
}
