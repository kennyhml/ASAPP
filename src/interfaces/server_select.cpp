#include <iostream>
#include "asa/ui/server_select.h"
#include "asa/utility.h"
#include "asa/core/state.h"

namespace asa
{
    bool server_select::is_best_result_selected() const
    {
        static cv::Vec3b selected{128, 64, 2};
        static cv::Vec3b hovered{83, 39, 1};
        static cv::Vec3b mods_color{135, 79, 23};

        if (cv::countNonZero(utility::mask(best_result_.area, selected, 15)) > 150) {
            return true;
        }
        // If the server has mods enabled selecting it will bring a screen up
        if (cv::countNonZero(
                utility::mask(join_button_mods_popup_.area, mods_color, 15)) > 150) {
            return true;
        }
        return cv::countNonZero(utility::mask(best_result_.area, hovered, 15)) > 150;
    }

    bool server_select::is_open() const
    {
        return window::match(embedded::interfaces::refresh, refresh_button_.area);
    }

    bool server_select::can_join() const
    {
        static cv::Vec3b joinable{153, 77, 4};
        return cv::countNonZero(utility::mask(join_button_.area, joinable, 20)) > 200;
    }

    bool server_select::can_join_last_played() const
    {
        static cv::Vec3b last_played_color{142, 219, 231};
        return cv::countNonZero(
                   utility::mask(join_last_played_button_.area, last_played_color,
                                 20)) > 50;
    }

    bool server_select::is_joining_server() const
    {
        static cv::Vec3b red{255, 0, 0};
        static cv::Vec3b text{193, 245, 255};

        // TODO: This could mistaken a connection timeout error for joining
        cv::Mat mask = utility::mask(joining_text_2_, red, 20);
        if (cv::countNonZero(mask) > 150) { return true; }

        mask = utility::mask(joining_text_, text, 20);
        return cv::countNonZero(mask) > 150;
    }

    void server_select::join_server(const std::string& name)
    {
        std::cout << "[+] Joining server " << name << "..." << std::endl;
        searchbar_.press();
        checked_sleep(std::chrono::milliseconds(100));
        for (int i = 0; i < 10; ++i) {
            controls::key_press("backspace", std::chrono::milliseconds(20));
        }
        searchbar_.search_for(name);
        checked_sleep(std::chrono::seconds(3));

        bool refreshed = true;
        while (!is_best_result_selected()) {
            // Don't refresh on the initial search
            if (!refreshed) {
                // This will also close some connection failed popups
                std::cout << "\t[-] Refreshing server list." << std::endl;
                refresh_button_.press();
                checked_sleep(std::chrono::seconds(3));
            }
            best_result_.press();
            checked_sleep(std::chrono::milliseconds(500));
            refreshed = false;
        }

        std::cout << "\t[-] Best search result selected." << std::endl;
        const auto start = std::chrono::system_clock::now();
        while (!is_joining_server()) {
            std::cout << "\t[-] Checking if mods enabled..." << std::endl;
            if (server_has_mods_enabled()) {
                std::cout << "\t[-] Mods enabled, pressing JOIN on mods popup." <<
                        std::endl;
                join_button_mods_popup_.press();
            } else {
                join_button_.press();
            }
            checked_sleep(1s);

            if (utility::timedout(start, 60s)) {
                throw std::exception("Failed to find join server");
            }
        }

        std::cout << "\t[-] Now joining session..." << std::endl;
        if (!utility::await([this] { return !is_open(); }, std::chrono::seconds(90))) {
            throw std::exception("Failed to join server within 90 seconds.");
        }
        utility::await([] { return !is_playing_movie(); }, 90s);
        std::cout << "[+] Server joined successfully." << std::endl;
    }

    void server_select::join_last_played()
    {
        join_last_played_button_.press();
        checked_sleep(1s);
    }

    void server_select::refresh()
    {
        refresh_button_.press();
        checked_sleep(std::chrono::seconds(1));
    }

    bool server_select::server_has_mods_enabled() const
    {
        // If the server has mods enabled selecting it will bring a screen up
        static cv::Vec3b join_mods{135, 79, 23};
        return cv::countNonZero(
                   utility::mask(join_button_mods_popup_.area, join_mods, 15)) > 150;
    }
}
