#pragma once
#include "container.h"
#include "../interfaces/baseinventory.h"

namespace asa::structures
{
    inline std::vector<std::string> blue_crate_names = { "SwampCaveTier1", "IceCaveTier1", "waterCaveTier1" };
    inline std::vector<std::string> yellow_crate_names = { "QualityTier3", "SwampCaveTier2", "IceCaveTier2", "waterCaveTier2" };
    inline std::vector<std::string> red_crate_names = { "QualityTier4", "SwampCaveTier3", "IceCaveTier3", "waterCaveTier3" };

    class CaveLootCrate final : public Container
    {
    public:
        enum Quality
        {
            BLUE = 1,
            YELLOW = 2,
            RED = 4,
            ANY = BLUE | YELLOW | RED,
        };

        CaveLootCrate(int t_quality_flags) : Container("Loot Crate", 0),
                                             quality_flags(t_quality_flags) {};

        const Quality get_crate_quality();

    private:
        std::optional<asa::window::Rect> get_info_area();
        Quality get_quality_from_tooltip(const std::string& tooltip);
        int quality_flags;
    };
}
