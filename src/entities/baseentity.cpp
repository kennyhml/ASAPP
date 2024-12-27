#include "asa/entities/baseentity.h"
#include "asa/game/window.h"

namespace asa
{
    base_entity::base_entity(std::string t_name, std::unique_ptr<base_inventory> t_inv)
        : name_(std::move(t_name)),
          inventory_(t_inv ? std::move(t_inv) : std::make_unique<base_inventory>()) {}

    void base_entity::primary_attack()
    {
        post_press(get_action_mapping("Fire"));
        last_primary_attack_ = std::chrono::system_clock::now();
    }

    void base_entity::secondary_attack()
    {
        post_press(MouseButton::RIGHT);
        last_secondary_attack_ = std::chrono::system_clock::now();
    }

    void base_entity::walk(const std::string& key, const std::chrono::milliseconds duration)
    {
        post_press(key, duration);
        last_moved_ = std::chrono::system_clock::now();
    }

    void base_entity::jump()
    {
        post_press(get_action_mapping("Jump"));
        last_jumped_ = std::chrono::system_clock::now();
    }
}
