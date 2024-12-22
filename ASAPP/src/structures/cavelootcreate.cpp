#include "asa/util/util.h"
#include <iostream>
#include <opencv2/highgui.hpp>

#include "asa/game/resources.h"
#include "asa/structures/cavelootcrate.h"

namespace asa::structures
{
    namespace
    {
        const std::vector<std::string> BLUE_CRATES = {
            "SwampCaveTier1", "IceCaveTier1", "IceCaveTierl", "UnderwaterCaveTier1"
        };
        const std::vector<std::string> YELLOW_CRATES = {
            "QualityTier3", "SwampCaveTier2", "IceCaveTier2", "UnderwaterCaveTier2"
        };
        const std::vector<std::string> RED_CRATES = {
            "QualityTier4", "SwampCaveTier3", "IceCaveTier3", "UnderwaterCaveTier3"
        };

        constexpr window::Color tooltip_white{196, 196, 195};

        bool is_in_tier(const std::string& crate, const std::vector<std::string>& tier)
        {
            return std::ranges::any_of(tier, [crate](const auto& identifier) -> bool {
                return crate.find(identifier) != std::string::npos;
            });
        }
    }

    CaveLootCrate::Quality CaveLootCrate::get_crate_quality()
    {
        if (util::is_only_one_bit_set(quality_flags_)) {
            return static_cast<Quality>(quality_flags_);
        }

        const auto tooltip_area = get_info_area();
        if (!tooltip_area.has_value()) {
            std::cerr << "\t[!] Failed to locate crate tooltip area." << std::endl;
            return ANY;
        }

        const auto mask = get_mask(tooltip_area.value(), tooltip_white, 50);
        const std::string res = window::ocr_threadsafe(
            mask, tesseract::PSM_SINGLE_LINE, "");

        std::cout << "[+] OCR result: " << res << std::endl;
        return get_quality_from_tooltip(res);
    }

    std::optional<window::Rect> CaveLootCrate::get_info_area()
    {
        const auto match_loc = locate_template(window::Rect(0, 0, 0, 0),
                                               resources::text::lootcrate);

        if (!match_loc.has_value()) { return std::nullopt; }

        return window::Rect(match_loc->x - 250, match_loc->y + match_loc->height + 2, 350,
                            30);
    }

    CaveLootCrate::Quality CaveLootCrate::get_quality_from_tooltip(const std::string& tooltip)
    {
        const std::string fixed = util::fix(tooltip, {{"Tierl", "Tier1"}});

        if (is_in_tier(fixed, BLUE_CRATES)) { return BLUE; }
        if (is_in_tier(fixed, YELLOW_CRATES)) { return YELLOW; }
        if (is_in_tier(fixed, RED_CRATES)) { return RED; }

        return ANY;
    }

    std::string CaveLootCrate::quality_to_string(const Quality quality)
    {
        switch (quality) {
        case RED: return "RED";
        case YELLOW: return "YELLOW";
        case BLUE: return "BLUE";
        case ANY: return "ANY";
        }
        return "";
    }

    CaveLootCrate::Quality CaveLootCrate::string_to_quality(const std::string& quality)
    {
        if (quality == "RED") { return RED; }
        if (quality == "YELLOW") { return YELLOW; }
        if (quality == "BLUE") { return BLUE; }

        return ANY;
    }
}
