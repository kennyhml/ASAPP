#include "asapp/items/item.h"
#include <iostream>
#include <opencv2/imgcodecs.hpp>
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

        std::string stringify_quality(const ItemData::ItemQuality quality)
        {
            switch (quality) {
            case ItemData::PRIMITIVE: return "Primitive";
            case ItemData::RAMSHACKLE: return "Ramshackle";
            case ItemData::APPRENTICE: return "Apprentice";
            case ItemData::JOURNEYMAN: return "Journeyman";
            case ItemData::MASTERCRAFT: return "Mastercraft";
            case ItemData::ASCENDANT: return "Ascendant";
            default: return "?";
            }
        }
    }

    Item::Item(std::string t_name, bool t_is_blueprint, ItemData::ItemQuality t_quality) :
        name_(std::move(t_name)),
        data_(ItemData(load_raw(name_), t_is_blueprint, t_quality))
    {
        if (!std::filesystem::exists(data_.icon_path)) {
            std::cerr << "[!] Invalid path: " << data_.icon_path << "\n";
        }
        icon_ = cv::imread(data_.icon_path.string(), cv::IMREAD_UNCHANGED);
    };

    std::string Item::info() const
    {
        switch (data_.type) {
        case ItemData::WEAPON:
        case ItemData::EQUIPPABLE:
        {
            std::string bp = data_.is_blueprint ? "Blueprint: " : "";
            return std::format("{}{} {}", bp, stringify_quality(data_.quality), name_);
        }
        case ItemData::STRUCTURE:
        case ItemData::ATTACHMENT:
        case ItemData::AMMO:
        {
            std::string bp = data_.is_blueprint ? "Blueprint: " : "";
            return std::format("{}{}", bp, name_);
        }
        default: return name_;
        }
    }

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
