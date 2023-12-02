#include "asapp/entities/dinoent.h"
#include "../core/wrappers.h"
#include "../util/util.h"

using namespace asa::entities;


void DinoEnt::Exit()
{
	this->inventory->Close();
	SleepFor(std::chrono::milliseconds(100));
}