#include "asa/entities/dinoentity.h"
#include "asa/core/state.h"
#include "asa/interfaces/hud.h"
#include "asa/util/util.h"

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
