#pragma once
#include "asapp/network/queries.h"
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

namespace asa::network
{
    namespace
    {
        // Holds the data the server list has in an array of json blocks
        const char* const SERVERLIST =
            "https://cdn2.arkdedicated.com/servers/asa/officialserverlist.json";


        size_t write_callback(void* content, const size_t size, const size_t nmemb,
                              std::string& out)
        {
            const size_t total_size = size * nmemb;
            out.append(static_cast<char*>(content), total_size);
            return total_size;
        }
    }

    std::optional<Server> get_server(const std::string& server_name)
    {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        CURL* curl = curl_easy_init();

        std::string response;

        // discard the version of the server name to avoid version mismatches.
        const size_t version_pos = server_name.find(" - (");
        std::string to_find = version_pos == std::string::npos
                                  ? server_name
                                  : server_name.substr(0, version_pos);
        // setup CURL
        curl_easy_setopt(curl, CURLOPT_URL, SERVERLIST);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        if (const CURLcode res = curl_easy_perform(curl); res != CURLE_OK) {
            std::cerr << "[!] Query failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return std::nullopt;
        }
        curl_easy_cleanup(curl);
        try {
            for (const nlohmann::json& data : nlohmann::json::parse(response)) {
                if (!data.contains("Name")) { continue; }
                const std::string name = data.at("Name");

                if (name.find(to_find) != std::string::npos) {
                    return Server::from_json(data);
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "[!] Parsing response failed: " << e.what() << std::endl;
        }
        return std::nullopt;
    }

    bool query(Server& server)
    {
        const std::optional<Server> other = get_server(server.name);

        if (!other.has_value()) {
            server.is_online = false;
            return false;
        }
        server = other.value();
        return true;
    }
}
