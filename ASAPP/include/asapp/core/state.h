#pragma once
#include <chrono>
#include <functional>

using namespace std::chrono_literals;

namespace asa::core
{
    using StateCheckCallback_t = std::function<void()>;

    void check_state();
    void sleep_for(std::chrono::milliseconds duration);
    void register_state_callback(StateCheckCallback_t callback);
    void remove_state_callback(StateCheckCallback_t callback);
}
