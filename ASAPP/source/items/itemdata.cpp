#include "asapp/items/itemdata.h"
#include "asapp/core/config.h"
#include "asapp/items/exceptions.h"

#define LOAD_FIELD(type, name, data, item_name) name(get_field<type>(data, item_name, #name))

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

        template <typename T>
        T get_field(const json& data, const std::string& item_name,
                    const std::string& field)
        {
            try { return data.at(field).get<T>(); }
            catch (std::out_of_range&) {
                throw MissingItemDataFieldError(item_name, field);
            }
        }
    }

    ItemData::ItemData(const std::string& t_name, const json& t_data,
                       const bool t_is_blueprint, const ItemQuality t_quality) :
        icon_path(
            core::config::assets_dir / get_field<std::string>(t_data, t_name, "icon")),
        type(item_type_map.at(get_field<std::string>(t_data, t_name, "type"))),
        quality(t_quality), LOAD_FIELD(float, weight, t_data, t_name),
        LOAD_FIELD(int, stack_size, t_data, t_name), is_blueprint(t_is_blueprint),
        LOAD_FIELD(bool, can_put_in_hotbar, t_data, t_name),
        LOAD_FIELD(bool, has_spoil_timer, t_data, t_name),
        LOAD_FIELD(bool, has_durability, t_data, t_name),
        LOAD_FIELD(bool, requires_engram, t_data, t_name),
        LOAD_FIELD(bool, has_ambiguous_query, t_data, t_name),
        LOAD_FIELD(bool, has_armor_value, t_data, t_name),
        LOAD_FIELD(bool, has_damage_value, t_data, t_name) {}
};
