#include "asa/items/items.h"

namespace asa
{
    namespace
    {
        nlohmann::json json_data;
        std::map<std::string, std::unique_ptr<item> > items;
    }

    const item& get_item(const std::string& name)
    {
        try {
            return *items.at(name);
        } catch (const std::out_of_range& e) {
            throw item_not_found(name);
        }
    }

    const std::map<std::string, std::unique_ptr<item> >& get_all_items()
    {
        return items;
    }

    void load_items()
    {
        json_data = nlohmann::json::parse(embedded::embedded_json);

        for (auto& [key, value]: json_data.items()) {
            items.emplace(key, std::make_unique<item>(key, item_data(key, value)));
        }
    }
}
