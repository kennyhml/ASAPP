#pragma once
#include <stdexcept>

namespace asa
{
    void set_crash_aware(bool aware);

    bool get_crash_aware();

    class asapp_error : public std::exception
    {
        using exception::exception;
    };

    class shooter_game_error : public std::exception
    {
    protected:
        std::string info;

    public:
        shooter_game_error(std::string info);

        const char* what() const noexcept override;
    };

    class server_crashed : public shooter_game_error
    {
    public:
        server_crashed();
    };

    class game_crashed : public shooter_game_error
    {
    public:
        game_crashed();
    };
}
