#include "asa/ui/console.h"
#include "asa/utility.h"
#include "asa/core/exceptions.h"
#include "asa/core/logging.h"

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
            post_press(get_action_mapping("ConsoleKeys"));
        } while (!utility::await([this]() -> bool { return is_open(); }, 5s));
    }

    void console::close()
    {
        if (!is_open()) { return; }

        do {
            post_press("enter");
        } while (!utility::await([this]() -> bool { return !is_open(); }, 5s));
    }

    void console::execute(const std::string& command)
    {
        const utility::stopwatch sw;
        get_logger()->info("Executing console command: '{}'..", command);

        // The benefit to blindly pressing the key is marginal, so its worth just
        // waiting for the console to have opened.
        open();
        if (last_command_ != command) {
            utility::set_clipboard(command);
            post_combination("ctrl", "v");
            last_command_ = command;
        } else {
            get_logger()->debug("Command matches previous command, using 'arrow up'..");
            post_press("up", 5ms);
        }

        do {
            if (sw.timedout(10s)) {
                throw asapp_error("Could not execute console command");
            }
            post_press("enter");
        } while (!utility::await([this] { return !is_open(); }, 5s));
        get_logger()->info("Command was executed within {}ms.", sw.elapsed<>().count());
    }
}
