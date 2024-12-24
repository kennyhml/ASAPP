#include "asa/core/managedthread.h"
#include <format>
#include <ranges>

#include "asa/core/exceptions.h"

namespace asa
{
    namespace
    {
        thread_registry_t registry;
        std::mutex registry_mutex;
    }

    managed_thread::managed_thread(std::string t_id, std::function<void()> t_target)
        : id_(std::move(t_id)), target_(std::move(t_target)) {}

    void managed_thread::start()
    {
        thread_ = std::make_unique<boost::thread>(target_);
        set_state(RUNNING);
    }

    managed_thread::ThreadState managed_thread::get_state() const
    {
        return state_;
    }

    bool managed_thread::has_started() const
    {
        return thread_ != nullptr;
    }

    bool managed_thread::has_terminated() const
    {
        return state_ == TERMINATED &&
               (!has_started() || thread_->try_join_for(boost::chrono::milliseconds(10)));
    }

    boost::thread* managed_thread::get_thread() const
    {
        return thread_.get();
    }

    const std::string& managed_thread::get_id() const
    {
        return id_;
    }

    void managed_thread::set_state(const ThreadState state)
    {
        state_ = state;
    }

    thread_interruped::thread_interruped(std::string t_thread_id, std::string t_why)
        : id(std::move(t_thread_id)), why(std::move(t_why)),
          what_(std::format("Thread '{}' was interrupted by '{}'!", id, why)) {}

    char const* thread_interruped::what() const
    {
        return what_.c_str();
    }

    void register_thread(const std::string& id, std::function<void()> target)
    {
        std::lock_guard lock(registry_mutex);
        // If we we were to replace the old thread without checking first it would cause
        // undefined behavior because std::terminate would be called in it.
        try {
            if (registry.contains(id) && !registry.at(id)->has_terminated()) {
                throw asapp_error(std::format(
                    "Failed to register thread '{}', thread already running.", id));
            }
        } catch (const std::out_of_range& e) {
            throw asapp_error(std::format("Failed to check thread '{}'.", id));
        }
        // we can safely place or replace the thread
        registry[id] = std::make_shared<managed_thread>(id, std::move(target));
    }

    std::shared_ptr<managed_thread> get_thread(const std::string& id)
    {
        std::lock_guard lock(registry_mutex);
        try {
            return registry.at(id);
        } catch (const std::out_of_range& e) {
            throw asapp_error(std::format("Thread '{}' is invalid!", id));
        }
    }

    std::shared_ptr<managed_thread> get_this_thread()
    {
        std::lock_guard lock(registry_mutex);
        for (const auto& thread: std::ranges::views::values(registry)) {
            if (boost::this_thread::get_id() == thread->get_thread()->get_id()) {
                return thread;
            }
        }
        return nullptr;
    }

    thread_registry_t& get_all_threads()
    {
        return registry;
    }

    void set_all_threads_state(managed_thread::ThreadState state,
                               const std::vector<std::string>& exclude)
    {
        std::lock_guard lock(registry_mutex);
        for (const auto& [id, thread]: registry) {
            if (std::ranges::find(exclude, id) != exclude.end()) {
                thread->set_state(state);
            }
        }
    }
}
