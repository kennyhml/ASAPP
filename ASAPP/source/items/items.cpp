#include "asapp/items/items.h"
#include <fstream>
#include <iostream>
#include "../util/util.h"
#include "asapp/core/config.h"
#include "asapp/items/exceptions.h"

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
            if (item) {
                delete item;
                item = nullptr;
            }
            else { add_to.push_back(&item); }

            item = new Item(name);
            std::cout << std::format("\t[-] Loaded  '{}' -> '{}'", item->get_name(),
                                     item->get_data().icon_path.string()) << "\n";
        }

        std::vector<Item**> all_resources;
        std::vector<Item**> all_consumables;
        std::vector<Item**> all_equippables;
        std::vector<Item**> all_weapons;
        std::vector<Item**> all_ammos;
    }


    bool init()
    {
        const auto start = std::chrono::system_clock::now();

        if (!load_raw_data()) { return false; }
        std::cout << "[+] Initializing item dummies...\n";
        if (!resources::init() || !consumables::init() || !equippables::init() || !
            weapons::init() || !ammo::init()) { return false; }

        const auto time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - start);
        std::cout << "-> Loaded all assets within " << time_taken << "\n";
        return true;
    }

    std::vector<std::vector<Item**>> iter_all()
    {
        return {
            resources::iter(), consumables::iter(), equippables::iter(), weapons::iter(),
            ammo::iter()
        };
    }

    bool resources::init()
    {
        try {
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
        catch (const ItemError& e) {
            std::cerr << "[!] Failed to load resource dummies. " << e.what() << "\n";
            return false;
        }
    }

    const std::vector<Item**>& resources::iter() { return all_resources; }

    bool consumables::init()
    {
        try {
            load(longrass, "Longrass", all_consumables);
            load(savoroot, "Savoroot", all_consumables);
            load(citronal, "Citronal", all_consumables);
            load(rockarrot, "Rockarrot", all_consumables);

            load(longrass_seed, "Longrass Seed", all_consumables);
            load(savoroot_seed, "Savoroot Seed", all_consumables);
            load(citronal_seed, "Citronal Seed", all_consumables);
            load(rockarrot_seed, "Rockarrot Seed", all_consumables);

            load(cooked_meat, "Cooked Meat", all_consumables);
            load(cooked_meat_jerky, "Cooked Meat Jerky", all_consumables);
            load(prime_meat_jerky, "Prime Meat Jerky", all_consumables);
            load(cooked_fish_meat, "Cooked Fish Meat", all_consumables);
            load(cooked_prime_fish_meat, "Cooked Prime Fish Meat", all_consumables);

            load(medical_brew, "Medical Brew", all_consumables);
            load(energy_brew, "Energy Brew", all_consumables);

            load(calien_soup, "Calien Soup", all_consumables);
            load(enduro_stew, "Enduro Stew", all_consumables);
            load(fria_curry, "Fria Curry", all_consumables);
            load(focal_chili, "Focal Chili", all_consumables);
            load(lazarus_chowder, "Lazarus Chowder", all_consumables);
            return true;
        }
        catch (const ItemError& e) {
            std::cerr << "[!] Failed to load consumable dummies. " << e.what() << "\n";
            return false;
        }
    }

    const std::vector<Item**>& consumables::iter() { return all_consumables; }

    bool equippables::init()
    {
        try {
            load(flak_helmet, "Flak Helmet", all_equippables);
            load(flak_chest, "Flak Chestpiece", all_equippables);
            load(flak_leggings, "Flak Leggings", all_equippables);
            load(flak_gauntlets, "Flak Gauntlets", all_equippables);
            load(flak_boots, "Flak Boots", all_equippables);

            load(cloth_hat, "Cloth Hat", all_equippables);
            load(cloth_shirt, "Cloth Shirt", all_equippables);
            load(cloth_pants, "Cloth Pants", all_equippables);
            load(cloth_gloves, "Cloth Gloves", all_equippables);
            load(cloth_boots, "Cloth Boots", all_equippables);

            load(hide_hat, "Hide Hat", all_equippables);
            load(hide_shirt, "Hide Shirt", all_equippables);
            load(hide_pants, "Hide Pants", all_equippables);
            load(hide_gloves, "Hide Gloves", all_equippables);
            load(hide_boots, "Hide Boots", all_equippables);

            load(fur_cap, "Fur Cap", all_equippables);
            load(fur_chestpiece, "Fur Chestpiece", all_equippables);
            load(fur_leggings, "Fur Leggings", all_equippables);
            load(fur_gauntlets, "Fur Gauntlets", all_equippables);
            load(fur_boots, "Fur Boots", all_equippables);

            load(riot_helmet, "Riot Helmet", all_equippables);
            load(riot_chest, "Riot Chestpiece", all_equippables);
            load(riot_leggings, "Riot Leggings", all_equippables);
            load(riot_gauntlets, "Riot Gauntlets", all_equippables);
            load(riot_boots, "Riot Boots", all_equippables);

            load(ghilie_mask, "Ghilie Mask", all_equippables);
            load(ghilie_chestpiece, "Ghilie Chestpiece", all_equippables);
            load(ghilie_leggings, "Ghilie Leggings", all_equippables);
            load(ghilie_gauntlets, "Ghilie Gauntlets", all_equippables);
            load(ghilie_boots, "Ghilie Boots", all_equippables);

            load(wooden_shield, "Wooden Shield", all_equippables);
            load(metal_shield, "Metal Shield", all_equippables);
            load(riot_shield, "Riot Shield", all_equippables);
            load(gasmask, "Gasmask", all_equippables);
            return true;
        }
        catch (const ItemError& e) {
            std::cerr << "[!] Failed to load equippable dummies. " << e.what() << "\n";
            return false;
        }
    }

    const std::vector<Item**>& equippables::iter() { return all_equippables; }

    bool weapons::init()
    {
        try {
            load(metal_hatchet, "Metal Hatchet", all_weapons);
            load(metal_pick, "Metal Pick", all_weapons);
            load(metal_sickle, "Metal Sickle", all_weapons);
            load(metal_pike, "Metal Pike", all_weapons);
            load(sword, "Sword", all_weapons);
            load(bow, "Bow", all_weapons);
            load(wooden_club, "Wooden Club", all_weapons);
            load(shotgun, "Shotgun", all_weapons);
            load(longneck_rifle, "Longneck Rifle", all_weapons);
            load(pump_action_shotgun, "Pump-Action Shotgun", all_weapons);
            load(fabricated_sniper_rifle, "Fabricated Sniper Rifle", all_weapons);
            load(compound_bow, "Compound Bow", all_weapons);
            load(rocket_launcher, "Rocket Launcher", all_weapons);
            load(assault_rifle, "Assault Rifle", all_weapons);
            load(fabricated_pistol, "Fabricated Pistol", all_weapons);
            load(electric_prod, "Electric Prod", all_weapons);
            load(harpoon_launcher, "Harpoon Launcher", all_weapons);
            load(bola, "Bola", all_weapons);
            load(grenade, "Grenade", all_weapons);
            load(improvised_explosive_device, "Improvised Explosive Device", all_weapons);
            load(scissors, "Scissors", all_weapons);
            load(magnifying_glass, "Magnifying Glass", all_weapons);
            load(poison_grenade, "Poison Grenade", all_weapons);
            load(smoke_grenade, "Smoke Grenade", all_weapons);
            load(lance, "Lance", all_weapons);
            return true;
        }
        catch (const ItemError& e) {
            std::cerr << "[!] Failed to load weapon dummies. " << e.what() << "\n";
            return false;
        }
    }

    const std::vector<Item**>& weapons::iter() { return all_weapons; }

    bool ammo::init()
    {
        try {
            load(advanced_bullet, "Advanced Bullet", all_ammos);
            load(advanced_rifle_bullet, "Advanced Rifle Bullet", all_ammos);
            load(advanced_sniper_bullet, "Advanced Sniper Bullet", all_ammos);
            load(simple_bullet, "Simple Bullet", all_ammos);
            load(simple_rifle_ammo, "Simple Rifle Ammo", all_ammos);
            load(simple_shotgun_ammo, "Simple Shotgun Ammo", all_ammos);
            load(stone_arrow, "Stone Arrow", all_ammos);
            load(tranq_arrow, "Tranq Arrow", all_ammos);
            load(rocket_propelled_grenade, "Rocket Propelled Grenade", all_ammos);
            load(spear_bolt, "Spear Bolt", all_ammos);
            return true;
        }
        catch (const ItemError& e) {
            std::cerr << "[!] Failed to load ammo dummies. " << e.what() << "\n";
            return false;
        }
    }

    const std::vector<Item**>& ammo::iter() { return all_ammos; }
}
