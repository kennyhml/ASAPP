#include "asapp/interfaces/components/slot.h"


namespace asa::interfaces::components
{
    [[nodiscard]] bool Slot::is_empty() const
    {
        const auto roi = window::Rect(area.x + 46, area.y + 69, 42, 14);
        static constexpr window::Color weight_text_color(128, 231, 255);

        const cv::Mat masked = get_mask(roi, weight_text_color, 35);
        return countNonZero(masked) < 10;
    }

    [[nodiscard]] bool Slot::has(items::Item& item) const
    {
        return window::match_template(area, item.get_inventory_icon(), 0.7f,
                                      item.get_inventory_icon_mask());
    }

    bool Slot::get_item(items::Item& item_out) const { return false; }
}
