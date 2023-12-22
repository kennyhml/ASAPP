#pragma once
#include <filesystem>
#include "item.h"

using json = nlohmann::json;

namespace asa::items
{
    inline json raw_data = NULL;

    bool init();
    std::vector<std::vector<Item**>> iter_all();
    Item* get_custom_item(std::string name);

    namespace resources
    {
        bool init();
        const std::vector<Item**>& iter();

        inline Item* metal = nullptr;
        inline Item* metal_ingot = nullptr;
        inline Item* cementing_paste = nullptr;
        inline Item* achatina_paste = nullptr;
        inline Item* fiber = nullptr;
        inline Item* flint = nullptr;
        inline Item* gunpowder = nullptr;
        inline Item* sparkpowder = nullptr;
        inline Item* obsidian = nullptr;
        inline Item* polymer = nullptr;
        inline Item* stone = nullptr;
        inline Item* thatch = nullptr;
        inline Item* wood = nullptr;
        inline Item* fertilizer = nullptr;
        inline Item* element = nullptr;
        inline Item* black_pearl = nullptr;
        inline Item* silica_pearls = nullptr;
        inline Item* crystal = nullptr;
        inline Item* electronics = nullptr;
        inline Item* oil = nullptr;
        inline Item* hide = nullptr;
    }

    namespace consumables
    {
        bool init();
        const std::vector<Item**>& iter();

        inline Item* longrass = nullptr;
        inline Item* rockarrot = nullptr;
        inline Item* citronal = nullptr;
        inline Item* savoroot = nullptr;

        inline Item* longrass_seed = nullptr;
        inline Item* rockarrot_seed = nullptr;
        inline Item* citronal_seed = nullptr;
        inline Item* savoroot_seed = nullptr;

        inline Item* cooked_meat = nullptr;
        inline Item* cooked_meat_jerky = nullptr;
        inline Item* prime_meat_jerky = nullptr;
        inline Item* cooked_fish_meat = nullptr;
        inline Item* cooked_prime_fish_meat = nullptr;
        inline Item* spoiled_meat = nullptr;

        inline Item* stimberry = nullptr;
        inline Item* mejoberry = nullptr;
        inline Item* narcoberry = nullptr;
        inline Item* amarberry = nullptr;
        inline Item* azulberry = nullptr;
        inline Item* cianberry = nullptr;
        inline Item* magenberry = nullptr;
        inline Item* tintoberry = nullptr;
        inline Item* verdberry = nullptr;

        inline Item* shadow_steak_saute = nullptr;
        inline Item* battle_tartare = nullptr;
        inline Item* medical_brew = nullptr;
        inline Item* energy_brew = nullptr;
        inline Item* calien_soup = nullptr;
        inline Item* enduro_stew = nullptr;
        inline Item* fria_curry = nullptr;
        inline Item* focal_chili = nullptr;
        inline Item* lazarus_chowder = nullptr;
    }

    namespace equippables
    {
        bool init();
        const std::vector<Item**>& iter();

        inline Item* cloth_hat = nullptr;
        inline Item* cloth_shirt = nullptr;
        inline Item* cloth_pants = nullptr;
        inline Item* cloth_gloves = nullptr;
        inline Item* cloth_boots = nullptr;

        inline Item* chitin_hat = nullptr;
        inline Item* chitin_shirt = nullptr;
        inline Item* chitin_pants = nullptr;
        inline Item* chitin_gloves = nullptr;
        inline Item* chitin_boots = nullptr;

        inline Item* hide_hat = nullptr;
        inline Item* hide_shirt = nullptr;
        inline Item* hide_pants = nullptr;
        inline Item* hide_gloves = nullptr;
        inline Item* hide_boots = nullptr;

        inline Item* fur_cap = nullptr;
        inline Item* fur_chestpiece = nullptr;
        inline Item* fur_leggings = nullptr;
        inline Item* fur_gauntlets = nullptr;
        inline Item* fur_boots = nullptr;

        inline Item* flak_helmet = nullptr;
        inline Item* flak_chest = nullptr;
        inline Item* flak_leggings = nullptr;
        inline Item* flak_gauntlets = nullptr;
        inline Item* flak_boots = nullptr;

        inline Item* riot_helmet = nullptr;
        inline Item* riot_chest = nullptr;
        inline Item* riot_leggings = nullptr;
        inline Item* riot_gauntlets = nullptr;
        inline Item* riot_boots = nullptr;

        inline Item* ghilie_mask = nullptr;
        inline Item* ghilie_chestpiece = nullptr;
        inline Item* ghilie_leggings = nullptr;
        inline Item* ghilie_gauntlets = nullptr;
        inline Item* ghilie_boots = nullptr;

        inline Item* wooden_shield = nullptr;
        inline Item* metal_shield = nullptr;
        inline Item* riot_shield = nullptr;
        inline Item* gasmask = nullptr;

        inline Item* scuba_mask = nullptr;
        inline Item* scuba_tank = nullptr;
        inline Item* scuba_leggings = nullptr;
        inline Item* scuba_flippers = nullptr;

        inline Item* pteranodon_saddle = nullptr;
        inline Item* ankylo_saddle = nullptr;
        inline Item* araneo_saddle = nullptr;
        inline Item* diplo_saddle = nullptr;
        inline Item* iguanodon_saddle = nullptr;
        inline Item* kapro_saddle = nullptr;
        inline Item* mammoth_saddle = nullptr;
        inline Item* megaloceros_saddle = nullptr;
        inline Item* sabertooth_saddle = nullptr;
        inline Item* sarco_saddle = nullptr;

        inline Item* argentavis_saddle = nullptr;
        inline Item* baryonyx_saddle = nullptr;
        inline Item* basilosaurus_saddle = nullptr;
        inline Item* beelzebufo_saddle = nullptr;
        inline Item* bronto_platform_saddle = nullptr;
        inline Item* bronto_saddle = nullptr;
        inline Item* carbonemys_saddle = nullptr;
        inline Item* carcharo_saddle = nullptr;
        inline Item* carno_saddle = nullptr;
        inline Item* castoroides_saddle = nullptr;
        inline Item* chalicotherium_saddle = nullptr;
        inline Item* daeodon_saddle = nullptr;
        inline Item* dire_bear_saddle = nullptr;
        inline Item* doedicurus_saddle = nullptr;
        inline Item* dunkleosteus_saddle = nullptr;
        inline Item* equus_saddle = nullptr;
        inline Item* gallimimus_saddle = nullptr;
        inline Item* giganotosaurus_saddle = nullptr;
        inline Item* ichthyosaurus_saddle = nullptr;
        inline Item* manta_saddle = nullptr;
        inline Item* megalania_saddle = nullptr;
        inline Item* megalodon_tek_saddle = nullptr;
        inline Item* megalodon_saddle = nullptr;
        inline Item* megalosaurus_saddle = nullptr;
        inline Item* megatherium_saddle = nullptr;
        inline Item* mosasaur_platform_saddle = nullptr;
        inline Item* mosasaur_saddle = nullptr;
        inline Item* mosasaur_tek_saddle = nullptr;
        inline Item* pachyrhinosaurus_saddle = nullptr;
        inline Item* pachy_saddle = nullptr;
        inline Item* paracer_platform_saddle = nullptr;
        inline Item* paracer_saddle = nullptr;
        inline Item* parasaur_saddle = nullptr;
        inline Item* pelagornis_saddle = nullptr;
        inline Item* phiomia_saddle = nullptr;
        inline Item* plesiosaur_saddle = nullptr;
        inline Item* procoptodon_saddle = nullptr;
        inline Item* pulmonsoscorpius_saddle = nullptr;
        inline Item* quetz_platform_saddle = nullptr;
        inline Item* quetz_saddle = nullptr;
        inline Item* raptor_saddle = nullptr;
        inline Item* rex_saddle = nullptr;
        inline Item* rex_tek_saddle = nullptr;
        inline Item* rhyniognatha_saddle = nullptr;
        inline Item* spino_saddle = nullptr;
        inline Item* stego_saddle = nullptr;
        inline Item* tapejara_saddle = nullptr;
        inline Item* tapejara_tek_saddle = nullptr;
        inline Item* terror_bird_saddle = nullptr;
        inline Item* therizinosaurus_saddle = nullptr;
        inline Item* thylacoleo_saddle = nullptr;
        inline Item* trike_saddle = nullptr;
        inline Item* tusoteuthis_saddle = nullptr;
        inline Item* woolly_rhino_saddle = nullptr;
        inline Item* yutyrannus_saddle = nullptr;
        inline Item* allosaurus_saddle = nullptr;
        inline Item* hyaenodon_saddle = nullptr;
    }

    namespace weapons
    {
        bool init();
        const std::vector<Item**>& iter();

        inline Item* metal_hatchet = nullptr;
        inline Item* metal_pick = nullptr;
        inline Item* metal_sickle = nullptr;
        inline Item* metal_pike = nullptr;
        inline Item* sword = nullptr;
        inline Item* bow = nullptr;
        inline Item* wooden_club = nullptr;
        inline Item* shotgun = nullptr;
        inline Item* longneck_rifle = nullptr;
        inline Item* pump_action_shotgun = nullptr;
        inline Item* fabricated_sniper_rifle = nullptr;
        inline Item* compound_bow = nullptr;
        inline Item* rocket_launcher = nullptr;
        inline Item* assault_rifle = nullptr;
        inline Item* fabricated_pistol = nullptr;
        inline Item* electric_prod = nullptr;
        inline Item* harpoon_launcher = nullptr;
        inline Item* bola = nullptr;
        inline Item* grenade = nullptr;
        inline Item* improvised_explosive_device = nullptr;
        inline Item* scissors = nullptr;
        inline Item* magnifying_glass = nullptr;
        inline Item* poison_grenade = nullptr;
        inline Item* smoke_grenade = nullptr;
        inline Item* lance = nullptr;
        inline Item* spyglass = nullptr;
        inline Item* torch = nullptr;
        inline Item* stone_hatchet = nullptr;
        inline Item* stone_pick = nullptr;
        inline Item* slingshot = nullptr;
        inline Item* flare_gun = nullptr;
        inline Item* simple_pistol = nullptr;
        inline Item* paintbrush = nullptr;
        inline Item* crossbow = nullptr;
        inline Item* c4_detonator = nullptr;
    }

    namespace ammo
    {
        bool init();
        const std::vector<Item**>& iter();

        inline Item* advanced_bullet = nullptr;
        inline Item* advanced_rifle_bullet = nullptr;
        inline Item* advanced_sniper_bullet = nullptr;
        inline Item* simple_bullet = nullptr;
        inline Item* simple_rifle_ammo = nullptr;
        inline Item* simple_shotgun_ammo = nullptr;
        inline Item* stone_arrow = nullptr;
        inline Item* tranq_arrow = nullptr;
        inline Item* rocket_propelled_grenade = nullptr;
        inline Item* spear_bolt = nullptr;
        inline Item* c4_charge = nullptr;
        inline Item* metal_arrow = nullptr;
    }
}
