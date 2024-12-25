#pragma once
#include <stdexcept>

namespace asa
{
    class asapp_error : public std::exception
    {
    public:
        explicit asapp_error(std::string t_info) : info_(std::move(t_info)) {}

        [[nodiscard]] char const* what() const override { return info_.c_str(); };

    protected:
        std::string info_;
    };

    class shooter_game_error : public std::exception
    {
    public:
        explicit shooter_game_error(std::string t_info) : info_(std::move(t_info)) {}

        [[nodiscard]] const char* what() const noexcept override { return info_.c_str(); }

    protected:
        std::string info_;
    };

    class server_crashed final : public shooter_game_error
    {
    public:
        server_crashed() : shooter_game_error("Disconnected from server!") {}
    };

    class game_crashed final : public shooter_game_error
    {
    public:
        game_crashed() : shooter_game_error("The game has crashed!") {}
    };
}
