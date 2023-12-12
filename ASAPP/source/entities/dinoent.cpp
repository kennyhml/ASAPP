#include "asapp/entities/dinoent.h"
#include "../core/wrappers.h"
#include "../util/util.h"

void asa::entities::DinoEnt::exit()
{
	inventory->Close();
	sleep_for(std::chrono::milliseconds(100));
}