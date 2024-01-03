#include "asapp/util/util.h"
#include <iostream>
#include "asapp/game/resources.h"
#include "asapp/structures/cavelootcrate.h"

namespace asa::structures
{
    const CaveLootCrate::Quality CaveLootCrate::get_crate_quality()
    {
        if (util::is_only_one_bit_set(quality_flags)) {
            std::cout << "\t[-] Skipped determining drop quality as it is fixed." <<
                std::endl;
            return static_cast<Quality>(quality_flags);
        }
        std::cout << "\t[-] Attempting to determine quality of Cave Loot Crate with "
            "several options..." << std::endl;

        auto tooltip_area = get_info_area();
        if (!tooltip_area.has_value()) {
            std::cerr << "\t[!] Failed to locate crate tooltip area." << std::endl;
            return ANY;
        }

        window::Color tooltip_white(196, 196, 195);
        auto maskedTooltip = get_mask(tooltip_area.value(), tooltip_white, 50);

        window::set_tesseract_image(maskedTooltip);
        window::tessEngine->SetPageSegMode(tesseract::PSM_SINGLE_LINE);

        std::string resultStr = window::tessEngine->GetUTF8Text();
        std::cout << "\t[-] OCR result:" << resultStr << std::endl;
        return get_quality_from_tooltip(resultStr);
    }

    std::optional<window::Rect> CaveLootCrate::get_info_area()
    {
        auto match_loc = locate_template(window::Rect(0, 0, 0, 0),
                                         resources::text::lootcrate);

        if (!match_loc.has_value()) { return std::nullopt; }

        return window::Rect(match_loc->x - 250, match_loc->y + match_loc->height + 2, 350,
                            30);
    }

    CaveLootCrate::Quality CaveLootCrate::get_quality_from_tooltip(
        const std::string& tooltip)
    {
        static std::vector<std::string> blue_crate_names = { "SwampCaveTier1", "IceCaveTier1", "IceCaveTierl", "waterCaveTier1", "waterCaveTierl" };
        static std::vector<std::string> yellow_crate_names = { "QualityTier3", "SwampCaveTier2", "IceCaveTier2", "waterCaveTier2" };
        static std::vector<std::string> red_crate_names = { "QualityTier4", "SwampCaveTier3", "IceCaveTier3", "waterCaveTier3" };
  
        auto is_in_tier = [tooltip](const std::vector<std::string>& names) -> bool {
            for (const auto& name : names) {
                if (tooltip.find(name) != std::string::npos) { return true; }
            }
            return false;
        };

        if (is_in_tier(blue_crate_names)) { return BLUE; }
        if (is_in_tier(yellow_crate_names)) { return YELLOW; }
        if (is_in_tier(red_crate_names)) { return RED; }

        return ANY;
    }
}
