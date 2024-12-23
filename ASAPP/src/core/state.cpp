#include "asa/core/state.h"
#include "../../include/asa/utility.h"
#include "asa/core/exceptions.h"
#include "asa/interfaces/main_menu.h"

namespace asa::core
{
    namespace
    {
        std::chrono::system_clock::time_point last_check;
        std::vector<StateCheckCallback_t> state_check_callbacks{
            []() {
                if (interfaces::main_menu->got_connection_timeout()) {
                    throw core::ServerCrashedError();
                }
            },
            {
                []() {
                    if (window::has_crashed_popup()) { throw core::GameCrashedError(); }
                }
            }
        };
    }

    void asa::core::check_state()
    {
        if (!util::timedout(last_check, std::chrono::seconds(1)) ||
            core::get_crash_aware()) { return; }

        last_check = std::chrono::system_clock::now();
        for (auto& callback : state_check_callbacks) { callback(); }
    }

    void asa::core::sleep_for(std::chrono::milliseconds duration)
    {
        check_state();
        return std::this_thread::sleep_for(duration);
    }

    void asa::core::register_state_callback(StateCheckCallback_t callback)
    {
        state_check_callbacks.push_back(callback);
    }
}
