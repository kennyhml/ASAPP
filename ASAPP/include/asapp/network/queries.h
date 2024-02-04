#pragma once
#include <optional>
#include "server.h"

namespace asa::network
{
    /**
     * @brief Retrieves a server from the official network api.
     *
     * @param server_name The name of the server to retrieve the data for.
     *
     * @return The server data if the server was found, otherwise std::nullopt.
     */
    std::optional<Server> get_server(const std::string& server_name);

    /**
     * @brief Queries the given server to update the data.
     *
     * @param server The server to update the data for, must be valid.
     *
     * @return True if the server was found an updated, false otherwise.
     */
    bool query(Server& server);
}
