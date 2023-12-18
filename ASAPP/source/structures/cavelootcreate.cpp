#include "../util/util.h"
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
        std::cout << "\t[-] OCR result:" << resultStr;
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

    const CaveLootCrate::Quality CaveLootCrate::get_quality_from_tooltip(
        std::string tooltip)
    {
        if (tooltip.find("SwampCaveTier1") != std::string::npos) {
            std::cout << "\t[+] Drop determined to be blue" << std::endl;
            return BLUE;
        }
        if (tooltip.find("QualityTier3") != std::string::npos || tooltip.
            find("SwampCaveTier2") != std::string::npos) {
            std::cout << "\t[+] Drop determined to be yellow" << std::endl;
            return YELLOW;
        }
        if (tooltip.find("QualityTier4") != std::string::npos || tooltip.
            find("SwampCaveTier3") != std::string::npos) {
            std::cout << "\t[+] Drop determined to be red" << std::endl;
            return RED;
        }

        return ANY;
    }
}
