#include "asa/utility.h"
#include "asa/items/item.h"
#include "asa/game/embedded.h"
#include "asa/items/exceptions.h"

#include <mutex>

namespace asa
{
    namespace
    {
        constexpr float SCALE_INV = 0.24f;
        constexpr float SCALE_NOTIF = 0.44f;
        constexpr float SCALE_NOTIF_EXPORT = 0.11f;

        std::string stringify(const item_data::ItemQuality quality)
        {
            switch (quality) {
                case item_data::PRIMITIVE: return "Primitive";
                case item_data::RAMSHACKLE: return "Ramshackle";
                case item_data::APPRENTICE: return "Apprentice";
                case item_data::JOURNEYMAN: return "Journeyman";
                case item_data::MASTERCRAFT: return "Mastercraft";
                case item_data::ASCENDANT: return "Ascendant";
                default:
                    return "";
            }
        }

        void convert(const cv::Mat& icon, cv::Mat& icon_rgb, cv::Mat& icon_rgba,
                     const bool resize, const float scale)
        {
            // If the asset is directly exported from the game files (such as the devkit)
            // it will be 256x256 with an alpha channel for the transparent background. For
            // 1920x1080 the ideal scale-down-factor is x0.24, if we made the item ourselves
            // (because the devkit one is bad) this step should not be done as the self
            // cropped asset wont be 256x256 to begin with.
            if (!resize) {
                icon.copyTo(icon_rgba);
            } else {
                const cv::Size new_size(icon.cols * scale, icon.rows * scale);
                cv::resize(icon, icon_rgba, new_size, 0, 0, cv::INTER_LINEAR);
            }
            // Create the inventory icon by removing the alpha channel, care that we KEEP
            // the RGBA inventory icon seperately since we need it to create our mask.
            cv::cvtColor(icon_rgba, icon_rgb, cv::COLOR_RGBA2RGB);
        }
    }


    item::item(std::string t_name, item_data t_data)
        : name_(std::move(t_name)), data_(std::move(t_data))
    {
        // Load the image from the embeded data (RGBA)
        try {
            icon_ = embedded::item_icon_map.at(name_);
        } catch (const std::out_of_range&) {
            throw item_icon_not_found(name_);
        };

        // Create the inventory icon and mask
        convert(icon_, inv_icon_, rgba_inv_icon_, is_exported(), SCALE_INV);
        inv_icon_mask_ = utility::mask_alpha_channel(rgba_inv_icon_);

        // Create the notification icon and mask
        const float sc = is_exported() ? SCALE_NOTIF_EXPORT : SCALE_NOTIF;
        convert(icon_, notif_icon_, rgba_notif_icon_, true, sc);
        notif_icon_mask_ = utility::mask_alpha_channel(rgba_notif_icon_);
    };

    item::item(const item& t_other, const bool t_is_blueprint,
               const item_data::ItemQuality t_quality) : item(t_other)
    {
        data_.is_blueprint = t_is_blueprint;
        data_.quality = t_quality;
    }

    std::string item::info() const
    {
        switch (data_.type) {
            case item_data::WEAPON:
            case item_data::EQUIPPABLE: {
                std::string bp = data_.is_blueprint ? "Blueprint: " : "";
                if (data_.quality != item_data::NONE) {
                    return std::format("{}{} {}", bp, stringify(data_.quality), name_);
                }
                return std::format("{}{}", bp, name_);
            }
            case item_data::STRUCTURE:
            case item_data::ATTACHMENT:
            case item_data::AMMO: {
                std::string bp = data_.is_blueprint ? "Blueprint: " : "";
                return std::format("{}{}", bp, name_);
            }
            case item_data::CONSUMABLE:
            case item_data::RESOURCE:
            case item_data::ARTIFACT: { break; }
        }
        return name_;
    }

    bool item::is_exported() const
    {
        return (icon_.cols == 256 && icon_.rows == 256);
    }
}
