#include "asapp/items/exceptions.h"
#include "asapp/items/item.h"

namespace asa::items
{
    ItemError::ItemError(const Item* t_item) : item_(t_item),
                                               info_(std::format(
                                                   "ItemError: Unspecified item error for {}",
                                                   t_item->get_name())) {}

    ItemError::ItemError(const Item* t_item, const std::string& t_message) :
        item_(t_item), info_(std::format("ItemError:  {}", t_message)) {}


    ItemDataNotFound::ItemDataNotFound(const std::string& t_item_name) : ItemError(
        nullptr, std::format("Failed to find item data for {}", t_item_name)) {}

    ItemIconNotFound::ItemIconNotFound(const std::filesystem::path& path) : ItemError(
        nullptr, std::format("Failed to load item icon at {}", path.string())) {}

    MissingItemDataFieldError::MissingItemDataFieldError(
        const std::string& t_item_name, const std::string& t_field_name) : ItemError(
        nullptr,
        std::format("Item '{}' is missing field '{}'", t_item_name, t_field_name)) {}
}
