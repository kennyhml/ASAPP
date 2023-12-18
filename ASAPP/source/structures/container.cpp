#include "asapp/structures/container.h"


namespace asa::structures
{
    Container::Container(std::string name, int slots, interfaces::BaseInventory* inv) :
        InteractableStructure(name, &settings::action_mappings::access_inventory, inv),
        max_slots(slots), inventory(inv)
    {
        if (!inventory) {
            inventory = new interfaces::BaseInventory(true);
            _interface = inventory;
        }
    }

    int Container::get_slot_count() { return info.get_fill_level() * max_slots; }
}
