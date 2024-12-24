#pragma once
#include "interface.h"
#include "asa/game/window.h"

#include <string>

namespace asa
{
    class console final : public interface
    {
    public:
        console() = default;

        /**
         * @brief Checks whether the console is currently open. 
         * \return 
         */
        [[nodiscard]] bool is_open() const override;

        /**
         * @brief Opens the console. 
         */
        void open();

        /**
         * @brief Closes the console. 
         */
        void close();

        /**
         * @brief Executes a command in the console.
         * 
         * @param command The command to execute.
         */
        void execute(const std::string& command);

    private:
        cv::Rect bar_lower_{0, 1052, 1919, 20};
    };
}
