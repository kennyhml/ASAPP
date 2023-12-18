#pragma once
#include "iinterfacecomponent.h"
#include "asapp/items/item.h"

namespace asa::interfaces::components
{
    struct Slot : IInterfaceComponent
    {
        Slot() : Slot(0, 0) {}
        Slot(const int t_x, const int t_y) : IInterfaceComponent(t_x, t_y, 86, 87) {}

        [[nodiscard]] bool is_empty() const;
        [[nodiscard]] bool has(items::Item& item) const;
        [[nodiscard]] bool get_item(items::Item& item_out) const;
    };
}
