#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include "asapp/util/util.h"
#include "asapp/items/item.h"
#include "asapp/items/exceptions.h"
#include "asapp/items/items.h"

namespace asa::items
{
    namespace
    {
        constexpr float SCALE_INV = 0.24f;
        constexpr float SCALE_NOFIF = 0.11f;

        json load_raw(const std::string& name)
        {
            if (raw_data.find(name) == raw_data.end()) { throw ItemDataNotFound(name); }
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
            }
            return "";
        }

        void convert(const cv::Mat& icon, cv::Mat& icon_rgb, cv::Mat& icon_rgba,
                     const bool is_exported, const float scale)
        {
            // If the asset is directley exported from the game files (such as the devkit)
            // it will be 256x256 with an alpha channel for the transparent background. For
            // 1920x1080 the ideal scale-down-factor is x0.24, if we made the item ourselves
            // (because the devkit one is bad) this step should not be done as the self
            // cropped asset wont be 256x256 to begin with.
            if (!is_exported) { icon.copyTo(icon_rgba); }
            else {
                const cv::Size new_size(icon.cols * scale, icon.rows * scale);
                cv::resize(icon, icon_rgba, new_size, 0, 0, cv::INTER_LINEAR);
            }
            // Create the inventory icon by removing the alpha channel, care that we KEEP
            // the RGBA inventory icon seperately since we need it to create our mask.
            cv::cvtColor(icon_rgba, icon_rgb, cv::COLOR_RGBA2RGB);
        }
    }

    Item::Item(std::string t_name, const bool t_is_blueprint,
               const ItemData::ItemQuality t_quality) : name_(std::move(t_name)),
                                                        data_(ItemData(
                                                            name_, load_raw(name_),
                                                            t_is_blueprint, t_quality))
    {
        if (!std::filesystem::exists(data_.icon_path)) {
            throw ItemIconNotFound(data_.icon_path);
        }
        icon_ = cv::imread(data_.icon_path.string(), cv::IMREAD_UNCHANGED);
    };

    Item::Item(const Item& t_other, const bool t_is_blueprint,
               const ItemData::ItemQuality t_quality) : Item(t_other)
    {
        data_.is_blueprint = t_is_blueprint;
        data_.quality = t_quality;
    }

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
        case ItemData::CONSUMABLE:
        case ItemData::RESOURCE:
        case ItemData::ARTIFACT: { break; }
        }
        return name_;
    }

    const cv::Mat& Item::get_inventory_icon()
    {
        if (inv_icon_.empty()) {
            convert(icon_, inv_icon_, rgba_inv_icon_, is_exported(), SCALE_INV);
        }
        return inv_icon_;
    }

    const cv::Mat& Item::get_inventory_icon_mask()
    {
        if (inv_icon_mask_.empty()) {
            if (rgba_inv_icon_.empty()) { get_inventory_icon(); }
            inv_icon_mask_ = util::mask_alpha_channel(rgba_inv_icon_);
        }
        return inv_icon_mask_;
    }

    const cv::Mat& Item::get_notification_icon()
    {
        if (notif_icon_.empty()) {
            convert(icon_, notif_icon_, rgba_notif_icon_, is_exported(), SCALE_NOFIF);
        }
        return notif_icon_;
    }

    const cv::Mat& Item::get_notification_icon_mask()
    {
        if (notif_icon_mask_.empty()) {
            if (rgba_notif_icon_.empty()) { get_notification_icon(); }
            notif_icon_mask_ = util::mask_alpha_channel(rgba_notif_icon_);
        }
        return notif_icon_mask_;
    }

    bool Item::is_exported() const { return (icon_.cols == 256 && icon_.rows == 256); }
}
