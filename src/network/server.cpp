#include "asa/network/server.h"

namespace asa::network
{
    server server::from_json(const nlohmann::json& data)
    {
        return {data.at("Name"), data.at("SessionName"), data.at("IP"),
                      data.at("Port"), data.at("ServerPing"),
                      std::stoi(data.at("DayTime").get<std::string>()),
                      data.at("NumPlayers"), data.at("BuildId"), data.at("MinorBuildId"),
                      true, data.at("IsOfficial").get<std::string>() == "1",
                      data.at("SessionIsPve") == 1};
    }
}
