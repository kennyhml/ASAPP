#include "asa/interfaces/components/tooltip.h"

#include "asa/game/resources.h"


namespace asa::interfaces::components
{
    item_tooltip item_tooltip::from_hovered(const window::Rect& area)
    {
        item_tooltip ret(area);
        ret.take_image();
        return ret;
    }

    std::optional<window::Rect> item_tooltip::get_durability_area() const
    {
        if (image_.empty()) { return std::nullopt; }

        const auto res = window::locate_template(image_, resources::text::durability);
        if (!res.has_value()) { return std::nullopt; }

        return window::Rect(res->x + res->width, res->y - 2, res->width * 1.2, res->height + 4);
    }
}
