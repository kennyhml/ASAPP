#include <iostream>
#include "asapp/interfaces/serverselect.h"
#include "asapp/util/util.h"
#include "asapp/core/state.h"
#include "asapp/game/resources.h"

namespace asa::interfaces
{
    bool ServerSelect::is_best_result_selected() const
    {
        static constexpr window::Color selected{128, 64, 2};
        static constexpr window::Color hovered{83, 39, 1};
        static constexpr window::Color mods_color{135, 79, 23};

        if (cv::countNonZero(window::get_mask(best_result_.area, selected, 15)) > 150) {
            return true;
        }
        // If the server has mods enabled selecting it will bring a screen up
        if (cv::countNonZero(
            window::get_mask(join_button_mods_popup_.area, mods_color, 15)) > 150) {
            return true;
        }
        return cv::countNonZero(window::get_mask(best_result_.area, hovered, 15)) > 150;
    }

    bool ServerSelect::is_open() const
    {
        return window::match_template(refresh_button_.area,
                                      resources::interfaces::refresh);
    }

    bool ServerSelect::can_join() const
    {
        static constexpr window::Color joinable{153, 77, 4};
        return cv::countNonZero(window::get_mask(join_button_.area, joinable, 20)) > 200;
    }

    bool ServerSelect::can_join_last_played() const
    {
        static constexpr window::Color last_played_color{142, 219, 231};
        return cv::countNonZero(
            window::get_mask(join_last_played_button_.area, last_played_color, 20)) > 50;
    }

    bool ServerSelect::is_joining_server() const
    {
        static constexpr window::Color red{255, 0, 0};
        static constexpr window::Color text{193, 245, 255};

        // TODO: This could mistaken a connection timeout error for joining
        cv::Mat mask = window::get_mask(joining_text_2_, red, 20);
        if (cv::countNonZero(mask) > 150) { return true; }

        mask = window::get_mask(joining_text_, text, 20);
        return cv::countNonZero(mask) > 150;
    }

    void ServerSelect::join_server(const std::string& name)
    {
        std::cout << "[+] Joining server " << name << "..." << std::endl;
        searchbar_.press();
        core::sleep_for(std::chrono::milliseconds(100));
        for (int i = 0; i < 10; ++i) {
          controls::key_press("backspace", std::chrono::milliseconds(20));
        }
        searchbar_.search_for(name);
        core::sleep_for(std::chrono::seconds(3));

        bool refreshed = true;
        while (!is_best_result_selected()) {
          // Don't refresh on the initial search
          if (!refreshed) {
            // This will also close some connection failed popups
            std::cout << "\t[-] Refreshing server list." << std::endl;
            refresh_button_.press();
            core::sleep_for(std::chrono::seconds(3));
          }
          best_result_.press();
          core::sleep_for(std::chrono::milliseconds(500));
          refreshed = false;
        }

        std::cout << "\t[-] Best search result selected." << std::endl;
        const auto start = std::chrono::system_clock::now();
        while (!is_joining_server()) {
            std::cout << "\t[-] Checking if mods enabled..." << std::endl;
            if (server_has_mods_enabled()) {
                std::cout << "\t[-] Mods enabled, pressing JOIN on mods popup." << std::endl;
                join_button_mods_popup_.press();
            }
            else {
                join_button_.press();
            }
            core::sleep_for(1s);
            
            if (util::timedout(start, 60s)) {
              throw std::exception("Failed to find join server");
            }
        }

        std::cout << "\t[-] Now joining session..." << std::endl;
        if (!util::await([this] { return !is_open(); }, std::chrono::seconds(90))) {
            throw std::exception("Failed to join server within 90 seconds.");
        }
        util::await([] {return !window::is_playing_transition_movie(); }, 90s);
        std::cout << "[+] Server joined successfully." << std::endl;
    }

    void ServerSelect::join_last_played()
    {
        join_last_played_button_.press();
        core::sleep_for(1s);
    }

    void ServerSelect::refresh()
    {
        refresh_button_.press();
        core::sleep_for(std::chrono::seconds(1));
    }

    bool ServerSelect::server_has_mods_enabled() const
    {
        // If the server has mods enabled selecting it will bring a screen up
        static window::Color join_mods{135, 79, 23};
        return cv::countNonZero(
            window::get_mask(join_button_mods_popup_.area, join_mods, 15)) > 150;
    }
}
