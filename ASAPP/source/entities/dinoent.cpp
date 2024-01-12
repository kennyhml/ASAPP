#include "asapp/entities/dinoent.h"
#include "asapp/util/util.h"
#include "asapp/core/state.h"

void asa::entities::DinoEnt::exit()
{
    get_inventory()->close();
    core::sleep_for(std::chrono::milliseconds(100));
}
