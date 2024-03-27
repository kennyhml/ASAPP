#include "asapp/entities/baseentity.h"
#include "asapp/core/state.h"

namespace asa::entities
{
    void BaseEntity::primary_attack()
    {
        window::post_press(settings::fire);
        last_primary_attack_ = std::chrono::system_clock::now();
    }

    void BaseEntity::secondary_attack()
    {
        window::post_mouse_press(controls::RIGHT);
        last_secondary_attack_ = std::chrono::system_clock::now();
    }


    void BaseEntity::walk(const std::string& key, std::chrono::milliseconds duration)
    {
        controls::key_down(key, duration);
        controls::key_up(key);

        last_moved_ = std::chrono::system_clock::now();
    }

    void BaseEntity::jump()
    {
        window::press(settings::jump);

        last_jumped_ = std::chrono::system_clock::now();
    }



}
