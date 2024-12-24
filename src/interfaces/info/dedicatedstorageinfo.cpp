#include "asa/interfaces/info/dedicatedstorageinfo.h"
#include "asa/utility.h"

namespace asa
{
    bool dedicated_storage_info::withdraw(const WithdrawOption option)
    {
        if (!can_withdraw(option)) { return false; }
        get_button_of(option).press();
        return true;
    }

    bool dedicated_storage_info::can_withdraw(const WithdrawOption option)
    {
        static cv::Vec3b text_color{144, 222, 244};

        const auto img = window::screenshot(get_button_of(option).area);
        return utility::count_matches(img, text_color, 20) > 50;
    }

    button& dedicated_storage_info::get_button_of(const WithdrawOption option)
    {
        switch (option) {
            case FIRST: return withdraw_button1_;
            case SECOND: return withdraw_button2_;
            case THIRD: return withdraw_button3_;
            case STACK: return withdraw_stack_;
        }
        throw std::runtime_error("Could not find a matching button for the given option");
    }
}
