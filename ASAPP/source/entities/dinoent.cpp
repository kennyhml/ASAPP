#include "asapp/entities/dinoent.h"
#include "asapp/core/state.h"
#include "asapp/interfaces/hud.h"

namespace asa::entities
{
    bool DinoEnt::is_mounted() const { return interfaces::hud->mount_hud_available(); }
}
