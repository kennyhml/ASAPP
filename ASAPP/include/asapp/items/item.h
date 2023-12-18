#pragma once
#include "itemdata.h"
#include "asapp/game/resources.h"

namespace asa::items
{
    class Item
    {
    public:
        explicit Item(std::string t_name, bool t_is_blueprint = false,
                      ItemData::ItemQuality t_quality = ItemData::BASIC);

        [[nodiscard]] const ItemData& get_data() const { return data_; }
        [[nodiscard]] const std::string& get_name() const { return name_; }

        const cv::Mat& get_inventory_icon();
        const cv::Mat& get_inventory_icon_mask();
        const cv::Mat& get_notification_icon();
        const cv::Mat& get_notification_icon_mask();

    private:
        std::string name_;
        ItemData data_;

        cv::Mat icon_;
        cv::Mat rgba_inventory_icon_;
        cv::Mat inventory_icon_mask_;
        cv::Mat inventory_icon_;

        cv::Mat rgba_notification_icon_;
        cv::Mat notification_icon_mask_;
        cv::Mat notification_icon_;
    };
}
