#pragma once
#include "containerinfo.h"
#include "components/button.h"

namespace asa::interfaces
{
    class DedicatedStorageInfo : public asa::interfaces::ContainerInfo
    {
    public:
        enum WithdrawOption
        {
            FIRST,
            SECOND,
            THIRD,
            STACK
        };

        /**
         * @brief Presses the deposit button to deposit all items.
         */
        void deposit();

        /**
         * @brief Presses a given withdraw option to take items.
         * 
         * @param option The withdraw option to press.
         * 
         * @return True if the option was available and pressed, false otherwise. 
         */
        bool withdraw(WithdrawOption option);

        /**
         * @brief Checks whether the withdraw button for an option is available.
         * 
         * @param option The withdraw option to check.
         *
         * @remark The option is available if enough items to withdraw are in the dedi.
         * 
         * @return True if the option is available, false otherwise.
         */
        [[nodiscard]] bool can_withdraw(WithdrawOption option);

    private:
        components::Button withdraw_button3_{774, 662, 369, 39};
        components::Button withdraw_button2_{774, 710, 369, 39};
        components::Button withdraw_button1_{774, 758, 369, 39};

        components::Button withdraw_stack_{774, 814, 369, 39};
        components::Button withdraw_deposit_{774, 863, 369, 39};

        components::Button& get_button_of(WithdrawOption option);
    };
}
