#pragma once
#include <stdexcept>

namespace asa
{
    class asapp_error : public std::exception
    {
    public:
        explicit asapp_error(std::string info);
    protected:
        std::string info;
    };

    class shooter_game_error : public std::exception
    {
    protected:
        std::string info;

    public:
        explicit shooter_game_error(const std::string& info);

        [[nodiscard]] const char* what() const noexcept override;
    };

    class server_crashed final : public shooter_game_error
    {
    public:
        server_crashed();
    };

    class game_crashed final : public shooter_game_error
    {
    public:
        game_crashed();
    };
}
