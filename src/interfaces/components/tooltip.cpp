#include "asa/ui/components/tooltip.h"

namespace asa
{
    item_tooltip item_tooltip::from_hovered(const cv::Rect& area)
    {
        item_tooltip ret(area);
        ret.take_image();
        return ret;
    }

    std::optional<cv::Rect> item_tooltip::get_durability_area() const
    {
        if (image_.empty()) { return std::nullopt; }

        const auto res = window::locate(embedded::text::durability, image_);
        if (!res.has_value()) { return std::nullopt; }

        return cv::Rect(res->x + res->width, res->y - 2, res->width * 1.2, res->height + 4);
    }
}
