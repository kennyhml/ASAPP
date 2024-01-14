#include "asapp/entities/baseentity.h"

namespace asa::entities
{
    void BaseEntity::primary_attack() { window::post_press(settings::fire); }

    void BaseEntity::secondary_attack() { window::post_mouse_press(controls::RIGHT); }
}
