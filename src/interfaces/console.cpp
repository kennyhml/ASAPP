#include "asa/ui/console.h"
#include "asa/utility.h"

namespace asa
{
    bool console::is_open() const
    {
        static cv::Vec3b gray{140, 140, 140};

        return utility::count_matches(bar_lower_, gray, 5) > 500;
    }

    void console::open()
    {
        if (is_open()) { return; }

        do {
            window::press(get_action_mapping("ConsoleKeys"));
        } while (!utility::await([this]() -> bool { return is_open(); }, 5s));
    }

    void console::close()
    {
        if (!is_open()) { return; }

        do { window::press("enter"); } while (!utility::await(
            [this]() -> bool { return !is_open(); }, 5s));
    }

    void console::execute(const std::string& command)
    {
        open();
        if (last_command_ != command) {
            utility::set_clipboard(command);
            controls::key_combination_press("ctrl", "v");
            last_command_ = command;
        } else { controls::key_press("up", 5ms); }

        // Keep trying to press enter until console closed
        int counter = 0;
        do {
            // Only try to close the console 12 times (aka 1min)
            if (counter > 12) { break; }
            window::press("enter");
            counter++;
        } while (!utility::await([this] { return !is_open(); }, 5s));
    }
}
