#pragma once
#include <exception>
#include <string>

namespace asa
{
    struct destination_not_found : std::exception
    {
        explicit destination_not_found(const std::string& t_dst_name);

        [[nodiscard]] char const* what() const override { return info_.c_str(); };

    private:
        std::string info_;
    };

    struct destination_not_ready : std::exception
    {
        explicit destination_not_ready(const std::string& t_dst_name);

        [[nodiscard]] char const* what() const override { return info_.c_str(); };

    private:
        std::string info_;
    };
}
