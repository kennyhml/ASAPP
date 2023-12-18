#include "asapp/core/exceptions.h"

namespace asa::core
{
    bool crash_aware = false;

    void set_crash_aware(bool aware) { crash_aware = aware; }
    bool get_crash_aware() { return crash_aware; }

    ShooterGameError::ShooterGameError(std::string info) : info(
        "ShooterGameError: " + info) {};

    const char* ShooterGameError::what() const noexcept { return info.c_str(); }

    ServerCrashedError::ServerCrashedError() : ShooterGameError(
        "Server / connection crashed.") {};

    GameCrashedError::GameCrashedError() : ShooterGameError("The game has crashed.") {};
}
