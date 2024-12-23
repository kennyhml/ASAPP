#pragma once
#include <filesystem>
#include "item.h"

using json = nlohmann::json;

namespace asa
{
    inline json raw_data = NULL;

    bool init();
    std::map<ItemData::ItemType ,std::vector<item**>> iter_all();
    item* get_custom_item(std::string name);

    namespace resources
    {
        bool init();
        const std::vector<item**>& iter();

        inline item* metal = nullptr;
        inline item* metal_ingot = nullptr;
        inline item* cementing_paste = nullptr;
        inline item* achatina_paste = nullptr;
        inline item* fiber = nullptr;
        inline item* chitin = nullptr;
        inline item* flint = nullptr;
        inline item* gunpowder = nullptr;
        inline item* sparkpowder = nullptr;
        inline item* obsidian = nullptr;
        inline item* polymer = nullptr;
        inline item* stone = nullptr;
        inline item* thatch = nullptr;
        inline item* wood = nullptr;
        inline item* fertilizer = nullptr;
        inline item* element = nullptr;
        inline item* black_pearl = nullptr;
        inline item* silica_pearls = nullptr;
        inline item* crystal = nullptr;
        inline item* electronics = nullptr;
        inline item* oil = nullptr;
        inline item* hide = nullptr;
        inline item* sap = nullptr;
        inline item* charcoal = nullptr;
        inline item* gasoline = nullptr;
    }

    namespace consumables
    {
        bool init();
        const std::vector<item**>& iter();

        inline item* longrass = nullptr;
        inline item* rockarrot = nullptr;
        inline item* citronal = nullptr;
        inline item* savoroot = nullptr;

        inline item* longrass_seed = nullptr;
        inline item* rockarrot_seed = nullptr;
        inline item* citronal_seed = nullptr;
        inline item* savoroot_seed = nullptr;

        inline item* cooked_meat = nullptr;
        inline item* cooked_meat_jerky = nullptr;
        inline item* prime_meat_jerky = nullptr;
        inline item* cooked_fish_meat = nullptr;
        inline item* cooked_prime_fish_meat = nullptr;
        inline item* spoiled_meat = nullptr;

        inline item* stimberry = nullptr;
        inline item* mejoberry = nullptr;
        inline item* narcoberry = nullptr;
        inline item* amarberry = nullptr;
        inline item* azulberry = nullptr;
        inline item* cianberry = nullptr;
        inline item* magenberry = nullptr;
        inline item* tintoberry = nullptr;
        inline item* verdberry = nullptr;

        inline item* shadow_steak_saute = nullptr;
        inline item* battle_tartare = nullptr;
        inline item* medical_brew = nullptr;
        inline item* energy_brew = nullptr;
        inline item* calien_soup = nullptr;
        inline item* enduro_stew = nullptr;
        inline item* fria_curry = nullptr;
        inline item* focal_chili = nullptr;
        inline item* lazarus_chowder = nullptr;
    }

    namespace equippables
    {
        bool init();
        const std::vector<item**>& iter();

        inline item* cloth_hat = nullptr;
        inline item* cloth_shirt = nullptr;
        inline item* cloth_pants = nullptr;
        inline item* cloth_gloves = nullptr;
        inline item* cloth_boots = nullptr;

        inline item* chitin_hat = nullptr;
        inline item* chitin_shirt = nullptr;
        inline item* chitin_pants = nullptr;
        inline item* chitin_gloves = nullptr;
        inline item* chitin_boots = nullptr;

        inline item* hide_hat = nullptr;
        inline item* hide_shirt = nullptr;
        inline item* hide_pants = nullptr;
        inline item* hide_gloves = nullptr;
        inline item* hide_boots = nullptr;

        inline item* fur_cap = nullptr;
        inline item* fur_chestpiece = nullptr;
        inline item* fur_leggings = nullptr;
        inline item* fur_gauntlets = nullptr;
        inline item* fur_boots = nullptr;

        inline item* flak_helmet = nullptr;
        inline item* flak_chest = nullptr;
        inline item* flak_leggings = nullptr;
        inline item* flak_gauntlets = nullptr;
        inline item* flak_boots = nullptr;

        inline item* riot_helmet = nullptr;
        inline item* riot_chest = nullptr;
        inline item* riot_leggings = nullptr;
        inline item* riot_gauntlets = nullptr;
        inline item* riot_boots = nullptr;

        inline item* ghilie_mask = nullptr;
        inline item* ghilie_chestpiece = nullptr;
        inline item* ghilie_leggings = nullptr;
        inline item* ghilie_gauntlets = nullptr;
        inline item* ghilie_boots = nullptr;

        inline item* wooden_shield = nullptr;
        inline item* metal_shield = nullptr;
        inline item* riot_shield = nullptr;
        inline item* gasmask = nullptr;

        inline item* scuba_mask = nullptr;
        inline item* scuba_tank = nullptr;
        inline item* scuba_leggings = nullptr;
        inline item* scuba_flippers = nullptr;

        inline item* pteranodon_saddle = nullptr;
        inline item* ankylo_saddle = nullptr;
        inline item* araneo_saddle = nullptr;
        inline item* diplo_saddle = nullptr;
        inline item* iguanodon_saddle = nullptr;
        inline item* kapro_saddle = nullptr;
        inline item* mammoth_saddle = nullptr;
        inline item* megaloceros_saddle = nullptr;
        inline item* sabertooth_saddle = nullptr;
        inline item* sarco_saddle = nullptr;

        inline item* argentavis_saddle = nullptr;
        inline item* baryonyx_saddle = nullptr;
        inline item* basilosaurus_saddle = nullptr;
        inline item* beelzebufo_saddle = nullptr;
        inline item* bronto_platform_saddle = nullptr;
        inline item* bronto_saddle = nullptr;
        inline item* carbonemys_saddle = nullptr;
        inline item* carcharo_saddle = nullptr;
        inline item* carno_saddle = nullptr;
        inline item* castoroides_saddle = nullptr;
        inline item* chalicotherium_saddle = nullptr;
        inline item* daeodon_saddle = nullptr;
        inline item* dire_bear_saddle = nullptr;
        inline item* doedicurus_saddle = nullptr;
        inline item* dunkleosteus_saddle = nullptr;
        inline item* equus_saddle = nullptr;
        inline item* gallimimus_saddle = nullptr;
        inline item* giganotosaurus_saddle = nullptr;
        inline item* ichthyosaurus_saddle = nullptr;
        inline item* manta_saddle = nullptr;
        inline item* megalania_saddle = nullptr;
        inline item* megalodon_tek_saddle = nullptr;
        inline item* megalodon_saddle = nullptr;
        inline item* megalosaurus_saddle = nullptr;
        inline item* megatherium_saddle = nullptr;
        inline item* mosasaur_platform_saddle = nullptr;
        inline item* mosasaur_saddle = nullptr;
        inline item* mosasaur_tek_saddle = nullptr;
        inline item* pachyrhinosaurus_saddle = nullptr;
        inline item* pachy_saddle = nullptr;
        inline item* paracer_platform_saddle = nullptr;
        inline item* paracer_saddle = nullptr;
        inline item* parasaur_saddle = nullptr;
        inline item* pelagornis_saddle = nullptr;
        inline item* phiomia_saddle = nullptr;
        inline item* plesiosaur_saddle = nullptr;
        inline item* procoptodon_saddle = nullptr;
        inline item* pulmonsoscorpius_saddle = nullptr;
        inline item* quetz_platform_saddle = nullptr;
        inline item* quetz_saddle = nullptr;
        inline item* raptor_saddle = nullptr;
        inline item* rex_saddle = nullptr;
        inline item* rex_tek_saddle = nullptr;
        inline item* rhyniognatha_saddle = nullptr;
        inline item* spino_saddle = nullptr;
        inline item* stego_saddle = nullptr;
        inline item* tapejara_saddle = nullptr;
        inline item* tapejara_tek_saddle = nullptr;
        inline item* terror_bird_saddle = nullptr;
        inline item* therizinosaurus_saddle = nullptr;
        inline item* thylacoleo_saddle = nullptr;
        inline item* trike_saddle = nullptr;
        inline item* tusoteuthis_saddle = nullptr;
        inline item* woolly_rhino_saddle = nullptr;
        inline item* yutyrannus_saddle = nullptr;
        inline item* allosaurus_saddle = nullptr;
        inline item* hyaenodon_saddle = nullptr;
    }

    namespace weapons
    {
        bool init();
        const std::vector<item**>& iter();

        inline item* metal_hatchet = nullptr;
        inline item* metal_pick = nullptr;
        inline item* metal_sickle = nullptr;
        inline item* metal_pike = nullptr;
        inline item* sword = nullptr;
        inline item* bow = nullptr;
        inline item* wooden_club = nullptr;
        inline item* shotgun = nullptr;
        inline item* longneck_rifle = nullptr;
        inline item* pump_action_shotgun = nullptr;
        inline item* fabricated_sniper_rifle = nullptr;
        inline item* compound_bow = nullptr;
        inline item* rocket_launcher = nullptr;
        inline item* assault_rifle = nullptr;
        inline item* fabricated_pistol = nullptr;
        inline item* electric_prod = nullptr;
        inline item* harpoon_launcher = nullptr;
        inline item* bola = nullptr;
        inline item* grenade = nullptr;
        inline item* improvised_explosive_device = nullptr;
        inline item* scissors = nullptr;
        inline item* magnifying_glass = nullptr;
        inline item* poison_grenade = nullptr;
        inline item* smoke_grenade = nullptr;
        inline item* lance = nullptr;
        inline item* spyglass = nullptr;
        inline item* torch = nullptr;
        inline item* stone_hatchet = nullptr;
        inline item* stone_pick = nullptr;
        inline item* slingshot = nullptr;
        inline item* flare_gun = nullptr;
        inline item* simple_pistol = nullptr;
        inline item* paintbrush = nullptr;
        inline item* crossbow = nullptr;
        inline item* c4_detonator = nullptr;
        inline item* chainsaw = nullptr;
        inline item* whip = nullptr;
    }

    namespace ammo
    {
        bool init();
        const std::vector<item**>& iter();

        inline item* advanced_bullet = nullptr;
        inline item* advanced_rifle_bullet = nullptr;
        inline item* advanced_sniper_bullet = nullptr;
        inline item* simple_bullet = nullptr;
        inline item* simple_rifle_ammo = nullptr;
        inline item* simple_shotgun_ammo = nullptr;
        inline item* stone_arrow = nullptr;
        inline item* tranq_arrow = nullptr;
        inline item* rocket_propelled_grenade = nullptr;
        inline item* spear_bolt = nullptr;
        inline item* c4_charge = nullptr;
        inline item* metal_arrow = nullptr;
    }
}
