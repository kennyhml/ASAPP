#pragma once
#include "crafting_station.h"

namespace asa
{
    class cave_loot_crate final : public crafting_station
    {
    public:
        enum Quality
        {
            BLUE = 1,
            YELLOW = 2,
            RED = 4,
            ANY = BLUE | YELLOW | RED,
        };

        explicit cave_loot_crate(const int t_quality_flags) :
            crafting_station("Loot Crate", 0), quality_flags_(t_quality_flags) {}

        Quality get_crate_quality();

        std::optional<window::Rect> get_info_area();

        static std::string quality_to_string(Quality);

        static Quality string_to_quality(const std::string&);

    private:
        Quality get_quality_from_tooltip(const std::string& tooltip);
        int quality_flags_;
    };
}
