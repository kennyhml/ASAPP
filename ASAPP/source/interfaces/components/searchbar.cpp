#include "asapp/interfaces/components/searchbar.h"
#include "../../util/util.h"
#include "asapp/core/state.h"
#include "asapp/game/controls.h"
#include "asapp/game/globals.h"

namespace asa::interfaces::components
{
    bool SearchBar::has_text_entered()
    {
        static window::Color text_color(134, 234, 255);

        auto mask = get_mask(this->area, text_color, 30);
        return cv::countNonZero(mask) > 50;
    }

    bool SearchBar::has_blinking_cursor() const
    {
        static window::Color text_color(134, 234, 255);

        auto mask = get_mask(this->area, text_color, 30);
        return cv::countNonZero(mask) > 15;
    }

    void SearchBar::search_for(std::string term)
    {
        this->press();
        core::sleep_for(std::chrono::milliseconds(200));
        this->searching = true;

        if (!globals::useWindowInput) {
            util::set_clipboard(term);
            controls::key_combination_press("ctrl", "v");
        }
        else {
            for (auto c : term) { if (globals::useWindowInput) { window::post_char(c); } }
        }

        if (!util::await([this]() { return this->has_text_entered(); },
                         std::chrono::seconds(5))) {
            std::cerr << "[!] Failed to search, trying again..." << std::endl;
            return this->search_for(term);
        }

        core::sleep_for(std::chrono::milliseconds(50));
        window::press("enter");

        searching = false;
        last_searched_term = term;
        text_entered = true;
    }

    void SearchBar::press() const
    {
        window::Point loc = this->area.get_random_location(8);

        do { post_mouse_press_at(loc, controls::LEFT); }
        while (!util::await([this]() { return this->has_blinking_cursor(); },
                            std::chrono::milliseconds(500)));
    }

    void SearchBar::delete_search()
    {
        this->press();

        if (globals::useWindowInput) {
            for (int i = 0; i < last_searched_term.size(); i++) {
                window::post_key_press("BackSpace", false);
                window::post_key_press("Delete", false);
            }
        }
        else {
            controls::key_combination_press("Ctrl", "a");
            core::sleep_for(std::chrono::milliseconds(40));
            controls::key_press("Delete");
        }

        core::sleep_for(std::chrono::milliseconds(50));
        window::press("enter");

        this->set_text_cleared();
    }
}
