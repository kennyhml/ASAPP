#include "asapp/items/itemdata.h"

#include "asapp/core/config.h"
#include "asapp/core/state.h"

namespace asa::items
{
    namespace
    {
        std::unordered_map<std::string, ItemData::ItemType> item_type_map = {
            {"CONSUMABLE", ItemData::CONSUMABLE}, {"EQUIPPABLE", ItemData::EQUIPPABLE},
            {"WEAPON", ItemData::WEAPON}, {"AMMO", ItemData::AMMO},
            {"STRUCTURE", ItemData::STRUCTURE}, {"RESOURCE", ItemData::RESOURCE},
            {"ATTACHMENT", ItemData::ATTACHMENT}, {"ARTIFACT", ItemData::ARTIFACT}
        };
    }

    ItemData::ItemData(json t_data, const bool t_is_blueprint,
                       const ItemQuality t_quality) :
        icon_path((core::config::assets_dir / t_data.at("icon"))),
        type(item_type_map.at(std::string(t_data.at("type")))), quality(t_quality),
        weight(t_data.at("weight")), stack_size(t_data.at("stack_size")),
        is_blueprint(t_is_blueprint), can_put_in_hotbar(t_data.at("can_put_in_hotbar")),
        has_spoil_timer(t_data.at("has_spoil_timer")),
        has_durability(t_data.at("has_durability")),
        requires_engram(t_data.at("requires_engram")),
        has_ambiguous_query(t_data.at("has_ambiguous_query")) {}
};
