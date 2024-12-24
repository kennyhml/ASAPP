#include "asa/structures/cave_loot_crate.h"
#include "asa/utility.h"
#include <iostream>

#include <iostream>

namespace asa
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

        constexpr cv::Vec3b tooltip_white{196, 196, 195};

        bool is_in_tier(const std::string& crate, const std::vector<std::string>& tier)
        {
            return std::ranges::any_of(tier, [crate](const auto& identifier) -> bool {
                return crate.find(identifier) != std::string::npos;
            });
        }
    }

    cave_loot_crate::Quality cave_loot_crate::get_crate_quality()
    {
        if (utility::is_only_one_bit_set(quality_flags_)) {
            return static_cast<Quality>(quality_flags_);
        }

        const auto tooltip_area = get_info_area();
        if (!tooltip_area.has_value()) {
            std::cerr << "\t[!] Failed to locate crate tooltip area." << std::endl;
            return ANY;
        }

        const auto mask = utility::mask(tooltip_area.value(), tooltip_white, 50);
        const std::string res = window::ocr_threadsafe(
            mask, tesseract::PSM_SINGLE_LINE, "");

        std::cout << "[+] OCR result: " << res << std::endl;
        return get_quality_from_tooltip(res);
    }

    std::optional<cv::Rect> cave_loot_crate::get_info_area()
    {
        const auto loc = window::locate(embedded::text::lootcrate, cv::Rect(0, 0, 0, 0));

        if (!loc.has_value()) { return std::nullopt; }
        return cv::Rect(loc->x - 250, loc->y + loc->height + 2, 350, 30);
    }

    cave_loot_crate::Quality cave_loot_crate::get_quality_from_tooltip(
        const std::string& tooltip)
    {
        const std::string fixed = utility::fix(tooltip, {{"Tierl", "Tier1"}});

        if (is_in_tier(fixed, BLUE_CRATES)) { return BLUE; }
        if (is_in_tier(fixed, YELLOW_CRATES)) { return YELLOW; }
        if (is_in_tier(fixed, RED_CRATES)) { return RED; }

        return ANY;
    }

    std::string cave_loot_crate::quality_to_string(const Quality quality)
    {
        switch (quality) {
            case RED: return "RED";
            case YELLOW: return "YELLOW";
            case BLUE: return "BLUE";
            case ANY: return "ANY";
        }
        return "";
    }

    cave_loot_crate::Quality cave_loot_crate::string_to_quality(
        const std::string& quality)
    {
        if (quality == "RED") { return RED; }
        if (quality == "YELLOW") { return YELLOW; }
        if (quality == "BLUE") { return BLUE; }

        return ANY;
    }
}
