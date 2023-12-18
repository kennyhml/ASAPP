#include "asapp/items/items.h"
#include <fstream>
#include <iostream>
#include "../util/util.h"
#include "asapp/core/config.h"

namespace asa::items
{
    bool load_raw_data()
    {
        std::ifstream f(core::config::itemdata_path);
        if (!f.is_open()) {
            std::cout << "Failed to open itemdata.json!" << std::endl;
            return false;
        }

        raw_data = json::parse(f);
        f.close();
        return true;
    }

    bool init()
    {
        auto start = std::chrono::system_clock::now();

        if (!load_raw_data()) { return false; }
        std::cout << "[+] Initializing predefined items..." << std::endl;
        if (!resources::init() || !consumables::init()) { return false; }

        auto timeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - start);
        std::cout << "-> Loaded all assets within " << timeTaken << std::endl;
        return true;
    }

    void load(Item*& item, std::string name)
    {
        if (raw_data.find(name) == raw_data.end()) {
            std::cout << "[!] No item data found for: " << name << std::endl;
            return;
        }

        if (item) {
            delete item;
            item = nullptr;
        }

        item = new Item(name);
        std::cout << std::format("\t[-] Loaded predef. item '{}'. Icon: '{}'",
                                 item->get_name(),
                                 item->get_data().icon_path.string()) << std::endl;
    }

    bool resources::init()
    {
        load(metal, "Metal");
        load(metal_ingot, "Metal Ingot");
        load(cementing_paste, "Cementing Paste");
        load(achatina_paste, "Achatina Paste");
        load(fiber, "Fiber");
        load(flint, "Flint");
        load(gunpowder, "Gunpowder");
        load(sparkpowder, "Sparkpowder");
        load(obsidian, "Obsidian");
        load(polymer, "Polymer");
        load(stone, "Stone");
        load(thatch, "Thatch");
        load(wood, "Wood");
        load(fertilizer, "Fertilizer");

        return true;
    }

    bool consumables::init()
    {
        load(longrass, "Longrass");
        load(savoroot, "Savoroot");
        load(citronal, "Citronal");
        load(rockarrot, "Rockarrot");

        load(longrass_seed, "Longrass Seed");
        load(savoroot_seed, "Savoroot Seed");
        load(citronal_seed, "Citronal Seed");
        load(rockarrot_seed, "Rockarrot Seed");
        return true;
    }
}
