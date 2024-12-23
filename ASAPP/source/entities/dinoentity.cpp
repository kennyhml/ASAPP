#include "asapp/entities/dinoentity.h"
#include "asapp/core/state.h"
#include "asapp/interfaces/hud.h"
#include "asapp/util/util.h"

namespace asa::entities
{
    bool DinoEntity::is_mounted() const { return interfaces::hud->mount_hud_available(); }

    void DinoEntity::open_inventory() const
    {
        if (get_inventory()->is_open()) { return; }

        do { window::press(settings::access_inventory); }
        while (!util::await([this]() { return get_inventory()->is_open(); },
                            std::chrono::seconds(10)));
    }
}
