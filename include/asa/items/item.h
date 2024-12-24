#pragma once
#include "itemdata.h"
#include "asa/game/window.h"

namespace asa
{
    class item
    {
    public:
        /**
         * @brief Constructs an Item object with the given parameters.
         *
         * @param t_name The name of the item.
         * @param t_data The data of the item.
         *
         */
        explicit item(std::string t_name, item_data t_data);

        /**
         * @brief Creates a copy of an item but as a blueprint or a different quality.
         *
         * @param t_other The item to copy the original data from.
         * @param t_is_blueprint Whether the new item is a blueprint (default = false).
         * @param t_quality The new quality of the item (default = item_data::PRIMITIVE).
         */
        item(const item& t_other, bool t_is_blueprint, item_data::ItemQuality t_quality);

        item(const item& t_other) = default;

        bool operator==(const item& other) const
        {
            return this->name_ == other.name_ && this->data_ == other.data_;
        }

        /**
         * @brief Gets a view at the data of the item.
         *
         * @return A reference to the items item_data object.
         */
        [[nodiscard]] const item_data& get_data() const { return data_; }

        /**
         * @brief Gets the items (in-game) name.
         *
         * @return The name of the item object.
         */
        [[nodiscard]] const std::string& get_name() const { return name_; }

        /** 
         * @brief Returns a string representing the item including blueprint and quality.
         *
         * @remark The output depends on the type of the item, a consumable cant be a blueprint
         * so the field is simply discarded.
         *
         * @return A string containing a quick overview on info about this item.
         */
        [[nodiscard]] std::string info() const;

        /**
         * @brief Checks whether the item is exported from the games real assets or not.
         *
         * @return true if the item is exported, false otherwise.
         */
        [[nodiscard]] bool is_exported() const;

        /** 
         * @brief Creates a mat (CV_8UC3) holding the items icon sized to match in inventories.
         *
         * @remark Gets created upon the first call and is stored on the heap for efficiency.
         * @remark The CV_8UC4 icon created as a byproduct is required to create the mask.
         * @remark The icon is guaranteed to be of uniform size for all exported assets.
         *
         * @return The icon of this item sized for the inventory.
         */
        const cv::Mat& get_inventory_icon();

        /** 
         * @brief Creates a bitmask (CV_8UC1) for the alpha channel of the inventory icon.
         *
         * @remark Gets created upon the first call and is stored on the heap for efficiency.
         * @remark The mask is guaranteed to be of the same size as the inventory icon.
         * @remark If the inventory icon has not been created, it will be called first.
         *
         * @return The mask of this item sized for the inventory.
         */
        const cv::Mat& get_inventory_icon_mask();

        /** 
         * @brief Creates a mat (CV_8UC3) holding the items icon sized to match notifications.
         *
         * @remark Gets created upon the first call and is stored on the heap for efficiency.
         * @remark The CV_8UC4 icon created as a byproduct is required to create the mask.
         * @remark The icon is guaranteed to be of uniform size for all exported assets.
         *
         * @return The icon of this item sized for the notifications.
         */
        const cv::Mat& get_notification_icon();

        /** 
         * @brief Creates a bitmask (CV_8UC1) for the alpha channel of the notification icon.
         *
         * @remark Gets created upon the first call and is stored on the heap for efficiency.
         * @remark The mask is guaranteed to be of the same size as the notfication icon.
         * @remark If the notification icon has not been created, it will be called first.
         *
         * @return The mask of this item sized for the notifications.
         */
        const cv::Mat& get_notification_icon_mask();

    private:
        std::string name_;
        item_data data_;

        cv::Mat icon_;
        cv::Mat rgba_inv_icon_;
        cv::Mat inv_icon_mask_;
        cv::Mat inv_icon_;

        cv::Mat rgba_notif_icon_;
        cv::Mat notif_icon_mask_;
        cv::Mat notif_icon_;
    };

    void load();

    const item& get_item(const std::string& name);

    const std::map<std::string, std::unique_ptr<item> >& get_all_items();
}
