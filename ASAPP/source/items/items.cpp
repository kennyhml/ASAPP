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
        if (!consumables::init() || !equippables::init() || !weapons::init() || !
            ammo::init()) { return false; }

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
            load(element, "Element", all_resources);

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
            load(shadow_steak_saute, "Shadow Steak Saute", all_consumables);
            load(spoiled_meat, "Spoiled Meat", all_consumables);
            load(battle_tartare, "Battle Tartare", all_consumables);
            load(stimberry, "Stimberry", all_consumables);
            load(mejoberry, "Mejoberry", all_consumables);
            load(narcoberry, "Narcoberry", all_consumables);
            load(amarberry, "Amarberry", all_consumables);
            load(azulberry, "Azulberry", all_consumables);
            load(cianberry, "Cianberry", all_consumables);
            load(magenberry, "Magenberry", all_consumables);
            load(tintoberry, "Tintoberry", all_consumables);
            load(verdberry, "Verdberry", all_consumables);

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

            load(chitin_hat, "Chitin Helmet", all_equippables);
            load(chitin_shirt, "Chitin Chest", all_equippables);
            load(chitin_pants, "Chitin Leggings", all_equippables);
            load(chitin_gloves, "Chitin Gauntlets", all_equippables);
            load(chitin_boots, "Chitin Boots", all_equippables);

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

            load(scuba_mask, "Scuba Mask", all_equippables);
            load(scuba_tank, "Scuba Tank", all_equippables);
            load(scuba_leggings, "Scuba Leggings", all_equippables);
            load(scuba_flippers, "Scuba Flippers", all_equippables);

            load(pteranodon_saddle, "Pteranodon Saddle", all_equippables);
            load(ankylo_saddle, "Ankylo Saddle", all_equippables);
            load(araneo_saddle, "Araneo Saddle", all_equippables);
            load(diplo_saddle, "Diplodocus Saddle", all_equippables);
            load(iguanodon_saddle, "Iguanodon Saddle", all_equippables);
            load(kapro_saddle, "Kaprosuchus Saddle", all_equippables);
            load(mammoth_saddle, "Mammoth Saddle", all_equippables);
            load(megaloceros_saddle, "Megaloceros Saddle", all_equippables);
            load(sabertooth_saddle, "Sabertooth Saddle", all_equippables);
            load(sarco_saddle, "Sarco Saddle", all_equippables);
            load(argentavis_saddle, "Argentavis Saddle", all_equippables);
            load(baryonyx_saddle, "Baryonyx Saddle", all_equippables);
            load(basilosaurus_saddle, "Basilosaurus Saddle", all_equippables);
            load(beelzebufo_saddle, "Beelzebufo Saddle", all_equippables);
            load(bronto_platform_saddle, "Bronto Platform Saddle", all_equippables);
            load(bronto_saddle, "Bronto Saddle", all_equippables);
            load(carbonemys_saddle, "Carbonemys Saddle", all_equippables);
            load(carcharo_saddle, "Carcharo Saddle", all_equippables);
            load(carno_saddle, "Carno Saddle", all_equippables);
            load(castoroides_saddle, "Castoroides Saddle", all_equippables);
            load(chalicotherium_saddle, "Chalicotherium Saddle", all_equippables);
            load(daeodon_saddle, "Daeodon Saddle", all_equippables);
            load(dire_bear_saddle, "Dire Bear Saddle", all_equippables);
            load(doedicurus_saddle, "Doedicurus Saddle", all_equippables);
            load(dunkleosteus_saddle, "Dunkleosteus Saddle", all_equippables);
            load(equus_saddle, "Equus Saddle", all_equippables);
            load(gallimimus_saddle, "Gallimimus Saddle", all_equippables);
            load(giganotosaurus_saddle, "Giganotosaurus Saddle", all_equippables);
            load(ichthyosaurus_saddle, "Ichthyosaurus Saddle", all_equippables);
            load(manta_saddle, "Manta Saddle", all_equippables);
            load(megalania_saddle, "Megalania Saddle", all_equippables);
            load(megalodon_tek_saddle, "Megalodon Tek Saddle", all_equippables);
            load(megalodon_saddle, "Megalodon Saddle", all_equippables);
            load(megalosaurus_saddle, "Megalosaurus Saddle", all_equippables);
            load(megatherium_saddle, "Megatherium Saddle", all_equippables);
            load(mosasaur_platform_saddle, "Mosasaur Platform Saddle", all_equippables);
            load(mosasaur_saddle, "Mosasaur Saddle", all_equippables);
            load(mosasaur_tek_saddle, "Mosasaur Tek Saddle", all_equippables);
            load(pachyrhinosaurus_saddle, "Pachyrhinosaurus Saddle", all_equippables);
            load(pachy_saddle, "Pachy Saddle", all_equippables);
            load(paracer_platform_saddle, "Paracer Platform Saddle", all_equippables);
            load(paracer_saddle, "Paracer Saddle", all_equippables);
            load(parasaur_saddle, "Parasaur Saddle", all_equippables);
            load(pelagornis_saddle, "Pelagornis Saddle", all_equippables);
            load(phiomia_saddle, "Phiomia Saddle", all_equippables);
            load(plesiosaur_saddle, "Plesiosaur Saddle", all_equippables);
            load(procoptodon_saddle, "Procoptodon Saddle", all_equippables);
            load(pulmonsoscorpius_saddle, "Pulmonoscorpius Saddle", all_equippables);
            load(quetz_platform_saddle, "Quetz Platform Saddle", all_equippables);
            load(quetz_saddle, "Quetz Saddle", all_equippables);
            load(raptor_saddle, "Raptor Saddle", all_equippables);
            load(rex_saddle, "Rex Saddle", all_equippables);
            load(rex_tek_saddle, "Rex Tek Saddle", all_equippables);
            load(rhyniognatha_saddle, "Rhyniognatha Saddle", all_equippables);
            load(spino_saddle, "Spino Saddle", all_equippables);
            load(stego_saddle, "Stego Saddle", all_equippables);
            load(tapejara_saddle, "Tapejara Saddle", all_equippables);
            load(tapejara_tek_saddle, "Tapejara Tek Saddle", all_equippables);
            load(terror_bird_saddle, "Terror Bird Saddle", all_equippables);
            load(therizinosaurus_saddle, "Therizinosaurus Saddle", all_equippables);
            load(thylacoleo_saddle, "Thylacoleo Saddle", all_equippables);
            load(trike_saddle, "Trike Saddle", all_equippables);
            load(tusoteuthis_saddle, "Tusoteuthis Saddle", all_equippables);
            load(woolly_rhino_saddle, "Woolly Rhino Saddle", all_equippables);
            load(yutyrannus_saddle, "Yutyrannus Saddle", all_equippables);
            load(allosaurus_saddle, "Allosaurus Saddle", all_equippables);
            load(hyaenodon_saddle, "Hyaenodon Saddle", all_equippables);

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
            load(metal_pike, "Pike", all_weapons);
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
            load(spyglass, "Spyglass", all_weapons);
            load(torch, "Torch", all_weapons);
            load(stone_hatchet, "Stone Hatchet", all_weapons);
            load(stone_pick, "Stone Pick", all_weapons);
            load(slingshot, "Slingshot", all_weapons);
            load(flare_gun, "Flare Gun", all_weapons);
            load(simple_pistol, "Simple Pistol", all_weapons);
            load(paintbrush, "Paintbrush", all_weapons);
            load(crossbow, "Crossbow", all_weapons);
            load(c4_detonator, "C4 Detonator", all_weapons);
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
            load(c4_charge, "C4 Charge", all_ammos);
            load(metal_arrow, "Metal Arrow", all_ammos);
            return true;
        }
        catch (const ItemError& e) {
            std::cerr << "[!] Failed to load ammo dummies. " << e.what() << "\n";
            return false;
        }
    }

    const std::vector<Item**>& ammo::iter() { return all_ammos; }
}
