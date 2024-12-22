#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace asa::network
{
    struct Server final
    {
    public:
        enum ServerType : int
        {
            OFFICIAL_1X,
            SMALLTRIBES,
            CLASSIC,
            ARKPOCALYPSE,
            CONQUEST,
        };

    public:
        std::string name;
        std::string session_name;
        std::string ip;

        int port;
        int ping;
        int day;
        int num_players;

        int build_id;
        int minor_build_id;

        bool is_online;
        bool is_official;
        bool is_pve;

        std::chrono::system_clock::time_point last_queried = std::chrono::system_clock::now();

        static Server from_json(const nlohmann::json& data);
    };
}
