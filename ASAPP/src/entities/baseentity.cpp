#include "asa/entities/baseentity.h"
#include "asa/core/state.h"

namespace asa
{
    void base_entity::primary_attack()
    {
        window::post_press(settings::fire);
        last_primary_attack_ = std::chrono::system_clock::now();
    }

    void base_entity::secondary_attack()
    {
        window::post_mouse_press(controls::RIGHT);
        last_secondary_attack_ = std::chrono::system_clock::now();
    }


    void base_entity::walk(const std::string& key, std::chrono::milliseconds duration)
    {
        controls::key_down(key, duration);
        controls::key_up(key);

        last_moved_ = std::chrono::system_clock::now();
    }

    void base_entity::jump()
    {
        window::press(settings::jump);

        last_jumped_ = std::chrono::system_clock::now();
    }
}
