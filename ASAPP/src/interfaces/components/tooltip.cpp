#include "asa/interfaces/components/tooltip.h"

#include "asa/game/resources.h"


namespace asa::interfaces::components
{
    ItemTooltip ItemTooltip::from_hovered(const window::Rect& area)
    {
        ItemTooltip ret(area);
        ret.take_image();
        return ret;
    }

    std::optional<window::Rect> ItemTooltip::get_durability_area() const
    {
        if (image_.empty()) { return std::nullopt; }

        const auto res = window::locate_template(image_, resources::text::durability);
        if (!res.has_value()) { return std::nullopt; }

        return window::Rect(res->x + res->width, res->y - 2, res->width * 1.2, res->height + 4);
    }
}
