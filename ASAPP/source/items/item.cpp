#include "asapp/items/item.h"
#include <iostream>
#include "../util/util.h"
#include "asapp/items/items.h"

namespace asa::items
{
    namespace
    {
        json load_raw(const std::string& name)
        {
            if (raw_data.find(name) == raw_data.end()) {
                throw std::runtime_error("Item not found: " + name);
            }
            return raw_data.at(name);
        }
    }

    Item::Item(std::string t_name, bool t_is_blueprint, ItemData::ItemQuality t_quality) :
        name_(std::move(t_name)),
        data_(ItemData(load_raw(name_), t_is_blueprint, t_quality)) {};

    const cv::Mat& Item::get_inventory_icon()
    {
        if (!inventory_icon_.empty()) { return inventory_icon_; }

        resize(icon_, rgba_inventory_icon_,
               cv::Size(icon_.cols * 0.24, icon_.rows * 0.24), 0, 0, cv::INTER_LINEAR);

        cvtColor(rgba_inventory_icon_, inventory_icon_, cv::COLOR_RGBA2RGB);
        return inventory_icon_;
    }

    const cv::Mat& Item::get_inventory_icon_mask()
    {
        if (!inventory_icon_mask_.empty()) { return inventory_icon_mask_; }

        // Make sure the rgba inventory icon has been created already
        if (rgba_inventory_icon_.empty()) { get_inventory_icon(); }

        inventory_icon_mask_ = util::mask_alpha_channel(rgba_inventory_icon_);
        return inventory_icon_mask_;
    }

    const cv::Mat& Item::get_notification_icon()
    {
        if (!notification_icon_.empty()) { return notification_icon_; }
        resize(icon_, rgba_notification_icon_,
               cv::Size(icon_.cols * 0.11, icon_.rows * 0.11), 0, 0, cv::INTER_LINEAR);

        cvtColor(rgba_notification_icon_, notification_icon_, cv::COLOR_RGBA2RGB);
        return notification_icon_;
    }

    const cv::Mat& Item::get_notification_icon_mask()
    {
        if (!notification_icon_mask_.empty()) { return notification_icon_mask_; }

        if (rgba_notification_icon_.empty()) { get_notification_icon(); }
        notification_icon_mask_ = util::mask_alpha_channel(rgba_notification_icon_);
        return notification_icon_mask_;
    }
}
