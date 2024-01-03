#include "asapp/core/config.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace asa::core::config
{
    bool set_environment(const std::filesystem::path& game_root_directory,
                         const std::filesystem::path& assets,
                         const std::filesystem::path& itemdata,
                         const std::filesystem::path& tessdata)
    {
        const bool environment_set = (set_game_directory(game_root_directory) &&
            set_assets_directory(assets) && set_itemdata_path(itemdata) &&
            set_tessdata_path(tessdata));
        if (!environment_set) {
            std::cerr << "[!] One or more environment variables is faulty. ASAPP "
                "was not initialized successfully.\n";
            return false;
        }
        std::cout << "[+] Environment variables set successfully.\n";
        return true;
    }


    bool set_environment(const std::filesystem::path& config_path)
    {
        std::ifstream f(config_path);
        if (!f.is_open()) {
            std::cerr << "[!] Failed to open config at " << config_path << "\n";
            return false;
        }
        json data = json::parse(f);
        f.close();
        return set_environment(data.at("gameBaseDirectory"), data.at("assetsDir"),
                               data.at("itemData"), data.at("tessdataPath"));
    }

    bool set_game_directory(std::filesystem::path path)
    {
        if (!std::filesystem::exists(path)) {
            std::cerr << "[!] Failed to set game directory. " << path <<
                " does not exist." << std::endl;
        }
        game_base_directory = path;
        return true;
    }

    bool set_assets_directory(std::filesystem::path path)
    {
        if (!std::filesystem::exists(path)) {
            std::cerr << "[!] Failed to set assets directory. " << path <<
                " does not exist." << std::endl;
        }
        assets_dir = path;
        return true;
    }

    bool set_itemdata_path(std::filesystem::path path)
    {
        if (!std::filesystem::exists(path)) {
            std::cerr << "[!] Failed to set itemdata.json path. " << path <<
                " does not exist." << std::endl;
        }
        itemdata_path = path;
        return true;
    }

    bool set_tessdata_path(std::filesystem::path path)
    {
        if (!std::filesystem::exists(path)) {
            std::cerr << "[!] Failed to set Tesseract data path. " << path <<
                " does not exist." << std::endl;
        }
        tessdata_path = path;
        return true;
    }
}
