#include "asapp/items/items.h"
#include <fstream>
#include <iostream>
#include "../util/util.h"
#include "asapp/core/config.h"

namespace asa::items
{
    namespace
    {
        bool load_raw_data()
        {
            std::ifstream f(core::config::itemdata_path);
            if (!f.is_open()) {
                std::cerr << "Failed to open itemdata.json!\n";
                return false;
            }

            raw_data = json::parse(f);
            f.close();
            return true;
        }

        void load(Item*& item, const std::string& name, std::vector<Item**>& add_to)
        {
            if (raw_data.find(name) == raw_data.end()) {
                std::cerr << "[!] No item data found for: " << name << "\n";
                return;
            }

            if (item) {
                delete item;
                item = nullptr;
            }
            else { add_to.push_back(&item); }

            item = new Item(name);
            std::cout << std::format("\t[-] Loaded predef. item '{}'. Icon: '{}'",
                                     item->get_name(),
                                     item->get_data().icon_path.string()) << "\n";
        }

        std::vector<Item**> all_resources;
        std::vector<Item**> all_consumables;
        std::vector<Item**> all_equippables;
    }

    bool init()
    {
        const auto start = std::chrono::system_clock::now();

        if (!load_raw_data()) { return false; }
        std::cout << "[+] Initializing predefined items...\n";
        if (!resources::init() || !consumables::init() || !equippables::init()) {
            return false;
        }

        const auto time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - start);
        std::cout << "-> Loaded all assets within " << time_taken << "\n";
        return true;
    }

    bool resources::init()
    {
        load(metal, "Metal", all_resources);
        load(metal_ingot, "Metal Ingot", all_resources);
        load(cementing_paste, "Cementing Paste", all_resources);
        load(achatina_paste, "Achatina Paste", all_resources);
        load(fiber, "Fiber", all_resources);
        load(flint, "Flint", all_resources);
        load(gunpowder, "Gunpowder", all_resources);
        load(sparkpowder, "Sparkpowder", all_resources);
        load(obsidian, "Obsidian", all_resources);
        load(polymer, "Polymer", all_resources);
        load(stone, "Stone", all_resources);
        load(thatch, "Thatch", all_resources);
        load(wood, "Wood", all_resources);
        load(fertilizer, "Fertilizer", all_resources);

        return true;
    }

    const std::vector<Item**>& resources::iter() { return all_resources; }

    bool consumables::init()
    {
        load(longrass, "Longrass", all_consumables);
        load(savoroot, "Savoroot", all_consumables);
        load(citronal, "Citronal", all_consumables);
        load(rockarrot, "Rockarrot", all_consumables);

        load(longrass_seed, "Longrass Seed", all_consumables);
        load(savoroot_seed, "Savoroot Seed", all_consumables);
        load(citronal_seed, "Citronal Seed", all_consumables);
        load(rockarrot_seed, "Rockarrot Seed", all_consumables);

        return true;
    }

    const std::vector<Item**>& consumables::iter() { return all_consumables; }


    bool equippables::init()
    {
        load(flak_helmet, "Flak Helmet", all_equippables);
        load(flak_chest, "Flak Chestpiece", all_equippables);
        load(flak_leggings, "Flak Leggings", all_equippables);
        load(flak_gauntlets, "Flak Gauntlets", all_equippables);
        load(flak_boots, "Flak Boots", all_equippables);
        return true;
    }

    const std::vector<Item**>& equippables::iter() { return all_equippables; }
}
