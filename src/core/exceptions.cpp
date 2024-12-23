#include "asa/core/exceptions.h"

namespace asa
{
    bool crash_aware = false;

    void set_crash_aware(const bool aware) { crash_aware = aware; }
    bool get_crash_aware() { return crash_aware; }

    shooter_game_error::shooter_game_error(const std::string& info)
        : info("shooter_game_error: " + info) {};

    const char* shooter_game_error::what() const noexcept { return info.c_str(); }

    server_crashed::server_crashed()
        : shooter_game_error("Server / connection crashed.") {};

    game_crashed::game_crashed()
        : shooter_game_error("The game has crashed.") {};
}
