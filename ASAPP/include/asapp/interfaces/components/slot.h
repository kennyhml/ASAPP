#pragma once
#include "iinterfacecomponent.h"
#include "asapp/items/item.h"

namespace asa::interfaces::components
{
    struct Slot : IInterfaceComponent
    {
        Slot() : Slot(0, 0) {}
        Slot(const int t_x, const int t_y) : IInterfaceComponent(t_x, t_y, 86, 87) {}

        [[nodiscard]] const window::Rect& get_stack_size_area() const;
        [[nodiscard]] const window::Rect& get_armor_or_damage_icon_area() const;
        [[nodiscard]] const window::Rect& get_spoil_or_durability_bar_area() const;
        [[nodiscard]] const window::Rect& get_weight_area() const;

        [[nodiscard]] bool is_empty() const;
        [[nodiscard]] bool has(items::Item& item, float* accuracy_out = nullptr) const;
        [[nodiscard]] bool get_item(items::Item*& item_out, bool verbose = false) const;

    private:
        [[nodiscard]] bool has_armor_value() const;
        [[nodiscard]] bool has_damage_value() const;
        [[nodiscard]] bool is_stack() const;
        [[nodiscard]] bool is_blueprint(items::ItemData data) const;
        [[nodiscard]] items::ItemData::ItemQuality get_quality() const;
    };
}
