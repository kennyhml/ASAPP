#include "asa/items/itemdata.h"
#include "asa/items/exceptions.h"

#define LOAD_FIELD(type, name, data, item_name) name(get_field<type>(data, item_name, #name))

namespace asa
{
    namespace
    {
        std::unordered_map<std::string, item_data::ItemType> item_type_map = {
            {"CONSUMABLE", item_data::CONSUMABLE}, {"EQUIPPABLE", item_data::EQUIPPABLE},
            {"WEAPON", item_data::WEAPON}, {"AMMO", item_data::AMMO},
            {"STRUCTURE", item_data::STRUCTURE}, {"RESOURCE", item_data::RESOURCE},
            {"ATTACHMENT", item_data::ATTACHMENT}, {"ARTIFACT", item_data::ARTIFACT}
        };

        template<typename T>
        T get_field(const nlohmann::json& data, const std::string& item_name,
                    const std::string& field)
        {
            try {
                return data.at(field).get<T>();
            } catch (std::out_of_range&) {
                throw missing_item_field(item_name, field);
            }
        }
    }

    item_data::item_data(const std::string& t_name, const nlohmann::json& t_data,
                         const bool t_is_blueprint, const ItemQuality t_quality)
        : icon_path(get_field<std::string>(t_data, t_name, "icon")),
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
