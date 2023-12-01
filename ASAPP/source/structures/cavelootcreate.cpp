#include "../util/util.h"
#include "asapp/structures/cavelootcrate.h"

using namespace asa::structures;


const CaveLootCrate::Quality CaveLootCrate::GetCrateQuality()
{
	if (util::IsOnlyOneBitSet(this->qualityFlags)) {
		std::cout << "[+] Skipped determining drop quality as it is fixed."
				  << std::endl;
		return static_cast<Quality>(this->qualityFlags);
	}
	std::cout << "[+] Attempting to determine quality of Cave Loot Crate with "
				 "several options..."
			  << std::endl;

	auto tooltipArea = this->GetInfoArea();
	if (!tooltipArea.has_value()) {
		std::cerr << "\t[!] Failed to locate crate tooltip area." << std::endl;
		return ANY;
	}

	window::Color tooltipWhite(196, 196, 195);
	auto maskedTooltip = window::GetMask(tooltipArea.value(), tooltipWhite, 50);

	window::SetTesseractImage(maskedTooltip);
	window::tessEngine->SetPageSegMode(tesseract::PSM_SINGLE_LINE);

	std::string resultStr = window::tessEngine->GetUTF8Text();
	std::cout << "\t[-] OCR result:" << resultStr;
	return GetQualityFromTooltip(resultStr);
}

std::optional<asa::window::Rect> CaveLootCrate::GetInfoArea()
{
	auto matchLoc = window::LocateTemplate(
		window::Rect(), resources::text::lootcrate);

	if (!matchLoc.has_value()) {
		return std::nullopt;
	}

	return window::Rect(
		matchLoc->x - 250, matchLoc->y + matchLoc->height + 2, 350, 30);
}

const CaveLootCrate::Quality CaveLootCrate::GetQualityFromTooltip(
	std::string tooltip)
{
	if (tooltip.find("SwampCaveTier1") != std::string::npos) {
		std::cout << "\t[+] Drop determined to be blue" << std::endl;
		return BLUE;
	}
	if (tooltip.find("QualityTier3") != std::string::npos ||
		tooltip.find("SwampCaveTier2") != std::string::npos) {
		std::cout << "\t[+] Drop determined to be yellow" << std::endl;
		return YELLOW;
	}
	else if (tooltip.find("QualityTier4") != std::string::npos ||
			 tooltip.find("SwampCaveTier3") != std::string::npos) {
		std::cout << "\t[+] Drop determined to be red" << std::endl;
		return RED;
	}

	return ANY;
}
