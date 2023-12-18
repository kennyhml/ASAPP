#pragma once
#include "container.h"
#include "../interfaces/baseinventory.h"

namespace asa::structures
{
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
        const Quality get_quality_from_tooltip(std::string tooltip);
        int quality_flags;
    };
}
