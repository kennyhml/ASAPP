#include "asapp/game/resources.h"
#include <iostream>
#include <format>
#include <opencv2/highgui.hpp>
#include "asapp/core/config.h"

#define LOAD_RESOURCE(dir, name)                                               \
	name = asa::resources::load_resource(dir / (std::string(#name) + ".png"))

namespace asa::resources
{
    namespace
    {
        bool is_valid_dir(std::filesystem::path path)
        {
            if (path.empty()) {
                std::cout << "[!] resources::init got empty path!" << std::endl;
                return false;
            }
            if (!std::filesystem::exists(path)) {
                std::cout << std::format("[!] Assets directory '{}' not found!",
                                         path.string()) << std::endl;
                return false;
            }
            return true;
        }
    }

    cv::Mat load_resource(std::filesystem::path path)
    {
        auto mat = cv::imread(path.string());
        std::cout << "\t[-] Resource loaded @ " << path << std::endl;
        return mat;
    }

    bool init()
    {
        if (!is_valid_dir(core::config::assets_dir)) { return false; }

        std::cout << "[+] Initializing resources..." << std::endl;
        if (!interfaces::init() || !text::init()) { return false; }

        return true;
    }

    bool interfaces::init()
    {
        auto dir = core::config::assets_dir / "interfaces";
        if (!is_valid_dir(dir)) {
            std::cout << dir << " not found" << std::endl;
            return false;
        }

        LOAD_RESOURCE(dir, cb_arrowdown);
        LOAD_RESOURCE(dir, regions);
        LOAD_RESOURCE(dir, lay_on);
        LOAD_RESOURCE(dir, day);
        LOAD_RESOURCE(dir, esc);
        LOAD_RESOURCE(dir, accept);
        LOAD_RESOURCE(dir, back);
        LOAD_RESOURCE(dir, join_last_session);
        LOAD_RESOURCE(dir, refresh);
        LOAD_RESOURCE(dir, armor);
        LOAD_RESOURCE(dir, damage);
        LOAD_RESOURCE(dir, deposit);
        LOAD_RESOURCE(dir, ride);
        LOAD_RESOURCE(dir, tribemanager);
        LOAD_RESOURCE(dir, day_log);

        return true;
    }

    bool text::init()
    {
        auto dir = core::config::assets_dir / "text";
        if (!is_valid_dir(dir)) {
            std::cout << dir << " not found" << std::endl;
            return false;
        }

        LOAD_RESOURCE(dir, added);
        LOAD_RESOURCE(dir, removed);
        LOAD_RESOURCE(dir, day);
        LOAD_RESOURCE(dir, lootcrate);
        LOAD_RESOURCE(dir, default_teleport);
        LOAD_RESOURCE(dir, fast_travel);
        LOAD_RESOURCE(dir, access_inventory);
        LOAD_RESOURCE(dir, x);
        LOAD_RESOURCE(dir, bp_weight);
        LOAD_RESOURCE(dir, detected_enemy);
        LOAD_RESOURCE(dir, pick_up);

        return true;
    }
}
