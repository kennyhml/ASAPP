#pragma once
#include "iinterface.h"
#include <string>

#include "asapp/game/window.h"

namespace asa::interfaces
{
    class Console final : public IInterface
    {
    public:
        Console() = default;

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
        window::Rect bar_lower_{0, 1052, 1919, 20};
    };

    inline std::unique_ptr<Console> console = std::make_unique<Console>();
}
