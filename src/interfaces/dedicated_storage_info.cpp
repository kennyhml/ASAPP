#include "../../include/asa/interfaces/info/dedicated_storage_info.h"

#include <opencv2/highgui.hpp>


namespace asa::interfaces
{
    bool dedicated_storage_info::withdraw(const WithdrawOption option)
    {
        if (!can_withdraw(option)) { return false; }
        get_button_of(option).press();
        return true;
    }

    bool dedicated_storage_info::can_withdraw(const WithdrawOption option)
    {
        static constexpr window::Color text_color{144, 222, 244};

        const auto button = get_button_of(option);
        const cv::Mat mask = window::get_mask(button.area, text_color, 30);
        return cv::countNonZero(window::get_mask(button.area, text_color, 20)) > 50;
    }

    components::Button& dedicated_storage_info::get_button_of(const WithdrawOption option)
    {
        switch (option) {
        case WithdrawOption::FIRST: return withdraw_button1_;
        case WithdrawOption::SECOND: return withdraw_button2_;
        case WithdrawOption::THIRD: return withdraw_button3_;
        case WithdrawOption::STACK: return withdraw_stack_;
        }

        throw std::runtime_error("Could not find a matching button for the given option");
    }
}
