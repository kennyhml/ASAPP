#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace asa::items
{
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
            PRIMITIVE,
            RAMSHACKLE,
            APPRENTICE,
            JOURNEYMAN,
            MASTERCRAFT,
            ASCENDANT
        };

    public:
        explicit ItemData(json t_data, bool t_is_blueprint = false,
                          ItemQuality t_quality = PRIMITIVE);

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
