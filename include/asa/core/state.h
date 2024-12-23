#pragma once
#include <chrono>
#include <functional>

using namespace std::chrono_literals;

namespace asa
{
    /**
     * @brief A callback that may be registered to perform frequent state checks.
     *
     * This function may be called in high frequency and should not block the executing
     * thread for a long time!
     *
     * If the function returns true, the state check is considered successful. Otherwise,
     * a `thread_interrupted` error is thrown.
     */
    using state_check_callback_t = std::function<bool()>;

    /**
     * @brief Sets the state crash aware to prevent a crash being detected while
     * a crash is actually being handled.
     *
     * @param aware Whether we are aware of a crash.
     */
    void set_crash_aware(bool aware);

    /**
     * @brief Checks if the state is currently crash aware, i.e a crash is currently
     * being handled.
     */
    [[nodiscard]] bool is_crash_aware();

    /**
     * @brief Checks the thread state and registered state callbacks.
     */
    void check_state();

    /**
     * @brief Performs a sleep bundled with a state check.
     *
     * @param duration The duration to sleep for.
     */
    void checked_sleep(std::chrono::milliseconds duration);

    /**
     * @brief Registers a new state check callback.
     *
     * @param id The ID of the callback to identify the cause of interruption.
     * @param callback The callback that will be called on state checks.
     */
    void register_state_callback(std::string id, state_check_callback_t callback);

    /**
    * @brief Removes a state check callback.
    *
    * @param id The callback to be removed.
    */
    void remove_state_callback(const std::string& id);

    bool is_playing_movie();
}
