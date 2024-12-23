#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace asa
{
    /**
     * @class ItemData
     * @brief Contains all relevant data of an item.
     */
    struct ItemData
    {
    public:
        enum ItemType
        {
            CONSUMABLE,
            EQUIPPABLE,
            WEAPON,
            AMMO,
            STRUCTURE,
            RESOURCE,
            ATTACHMENT,
            ARTIFACT,
        };

        enum ItemQuality
        {
            NONE,
            PRIMITIVE,
            RAMSHACKLE,
            APPRENTICE,
            JOURNEYMAN,
            MASTERCRAFT,
            ASCENDANT
        };

    public:
        /**
         * @brief Constructs an ItemData object with the given parameters.
         *
         * @param t_name The name of the item.
         * @param t_data The JSON data for the item.
         * @param t_is_blueprint (Optional) Whether the item is a blueprint. Default false.
         * @param t_quality (Optional) The quality of the item. Default PRIMITIVE.
         *
         * @return An ItemData object.
         */
        explicit ItemData(const std::string& t_name, const json& t_data,
                          bool t_is_blueprint = false, ItemQuality t_quality = PRIMITIVE);

        bool operator==(const ItemData&) const = default;

        std::filesystem::path icon_path;
        ItemType type;
        ItemQuality quality;

        float weight;
        int stack_size;

        bool is_blueprint;
        bool can_put_in_hotbar;
        bool has_spoil_timer;
        bool has_durability;
        bool requires_engram;
        bool has_ambiguous_query;
        bool has_armor_value;
        bool has_damage_value;
    };
}
