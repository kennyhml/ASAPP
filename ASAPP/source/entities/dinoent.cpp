#include "asapp/entities/dinoent.h"
#include "../core/wrappers.h"
#include "../util/util.h"

using namespace asa::entities;


DinoEnt::DinoEnt(std::string name, interfaces::BaseInventory* inv)
	: BaseEntity(name, inv){};

DinoEnt::DinoEnt(const char* name, interfaces::BaseInventory* inv)
	: BaseEntity(std::string(name), inv){};

void DinoEnt::Exit()
{
	this->inventory->Close();
	SleepFor(std::chrono::milliseconds(100));
}