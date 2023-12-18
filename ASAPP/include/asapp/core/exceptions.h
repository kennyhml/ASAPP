#pragma once
#include <stdexcept>

namespace asa::core
{
    void set_crash_aware(bool aware);
    bool get_crash_aware();

    class ShooterGameError : public std::exception
    {
    protected:
        std::string info;

    public:
        ShooterGameError(std::string info);

        const char* what() const noexcept override;
    };

    class ServerCrashedError : public ShooterGameError
    {
    public:
        ServerCrashedError();
    };

    class GameCrashedError : public ShooterGameError
    {
    public:
        GameCrashedError();
    };
}
