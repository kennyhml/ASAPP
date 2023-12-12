#include "asapp/entities/dinoent.h"
#include "../util/util.h"
#include "asapp/core/state.h"

void asa::entities::DinoEnt::exit()
{
	inventory->close();
	core::sleep_for(std::chrono::milliseconds(100));
}