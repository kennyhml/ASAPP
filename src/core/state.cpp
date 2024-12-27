#include "asa/utility.h"
#include "asa/core/managedthread.h"
#include "asa/core/state.h"

#include "asa/game/window.h"

namespace asa
{
    namespace
    {
        std::map<std::string, state_check_callback_t> state_check_callbacks;

        std::mutex erase_mutex;

        void pop_terminated_threads()
        {
            // Super important lock, the function may be called from multiple threads
            // at the same time and cause all kinds of issues without the lock!!
            std::lock_guard lock(erase_mutex);
            std::vector<std::string> to_erase;

            thread_registry_t& threads = get_all_threads();

            for (auto& [name, thread]: threads) {
                try {
                    // We cannot pop our own thread, its clearly still running
                    if (thread->get_thread()->get_id() == boost::this_thread::get_id()) {
                        continue;
                    }

                    // Add the thread for erasure if it hasnt started or is joinable
                    if (thread->has_terminated()) { to_erase.push_back(name); }
                } catch (const std::exception& e) {
                    // SK_SPDLOG_ERROR("", e.what());
                }
            }
            for (const auto& t: to_erase) { threads.erase(t); }
        }
    }

    void checked_sleep(const std::chrono::milliseconds duration)
    {
        pop_terminated_threads();

        const std::shared_ptr<managed_thread> thread = get_this_thread();
        if (thread) {
            const auto start = std::chrono::system_clock::now();
            while (!utility::timedout(start, duration)) {
                while (thread->get_state() == managed_thread::PAUSED) {
                    std::this_thread::sleep_for(10ms);
                }
                if (thread->get_state() == managed_thread::TERMINATED) {
                    throw thread_interruped(thread->get_id(), "TERMINATED");
                }
                std::this_thread::sleep_for(10ms);
            }
        }

        for (auto& [id, callback]: state_check_callbacks) {
            if (!callback()) {
                throw thread_interruped(thread->get_id(), id);
            }
        }
        std::this_thread::sleep_for(duration);
    }

    void register_state_callback(std::string id, state_check_callback_t callback)
    {
        state_check_callbacks.emplace(std::move(id), std::move(callback));
    }

    bool is_playing_movie()
    {
        return match(embedded::interfaces::server_transition,
                             cv::Rect{1569, 698, 342, 364});
    }
}
