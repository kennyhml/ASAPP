#include "../util/util.h"
#include "asapp/structures/cavelootcrate.h"

using namespace asa::structures;


const CaveLootCrate::Quality CaveLootCrate::GetCrateQuality()
{
	if (util::IsLessThanTwoBitsSet(this->qualityFlags)) {
		return this->qualityFlags;
	}
}