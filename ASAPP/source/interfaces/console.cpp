#include "asapp/interfaces/console.h"

#include <iostream>
#include <asapp/util/util.h>


namespace asa::interfaces
{
    bool Console::is_open() const
    {
        static constexpr window::Color gray{140,140,140};

        return cv::countNonZero(window::get_mask(bar_lower_, gray, 5)) > 500; 
    }

    void Console::open()
    {
        if (is_open()) { return; }

        do { window::press(settings::console); }
        while (!util::await([this]() -> bool { return is_open(); },
                            std::chrono::seconds(5)));
    }

    void Console::close()
    {
        if (!is_open()) { return; }

        do { window::press("enter"); }
        while (!util::await([this]() -> bool { return !is_open(); },
                            std::chrono::seconds(5)));
    }

    void Console::execute(const std::string& command)
    {
        open();
        util::set_clipboard(command);
        controls::key_combination_press("ctrl", "v");
        window::press("enter");
    }



}
