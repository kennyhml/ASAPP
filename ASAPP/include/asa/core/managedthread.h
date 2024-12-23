#pragma once
#include <functional>
#include <memory>
#include <boost/thread/thread.hpp>

namespace asa
{
    /**
     * @brief A wrapper around a boost thread that allows concrete control over a
     * certain thread addressed by an identifier.
     *
     * Provides simple methods to start, pause, resume and terminate individual threads
     * and obtain all their states.
     *
     * Thread state checks are performed through a call to `checked_sleep`.
     */
    struct managed_thread
    {
    public:
        /**
         * @brief The possible states a thread can be in. Initially READY.
         */
        enum ThreadState : int32_t
        {
            READY,
            PAUSED,
            TERMINATED,
            RUNNING
        };

        /**
         * @brief Creates a managed thread with a unique identifier and a target function.
         *
         * @param t_id The identifier of the thread, should be unique.
         * @param t_target The target function to be executed by the thread.
         */
        managed_thread(std::string t_id, std::function<void()> t_target);

        /**
         * @brief Starts the thread immediately.
         */
        void start();

        /**
         * @brief Gets the current state of the thread.
         */
        [[nodiscard]] ThreadState get_state() const;

        /**
         * @brief Checks whether this thread has been started.
         */
        [[nodiscard]] bool has_started() const;

        /**
         * @brief Checks whether this thread has (fully) terminated.
         */
        [[nodiscard]] bool has_terminated() const;

        /**
         * @brief Gets the underlying boost thread.
         */
        [[nodiscard]] boost::thread* get_thread() const;

        /**
         * @brief Gets the identifier that the thread was created with.
         */
        [[nodiscard]] const std::string& get_id() const;

        /**
         * @brief Sets the state to the provided state.
         *
         * @param state The state to set this thread to, e.g TERMINATED.
         */
        void set_state(ThreadState state);

    private:
        ThreadState state_ = READY;
        std::string id_;
        std::function<void()> target_;

        // Cannot inherit from thread because it starts on instantiation,
        // not the behavior we want
        std::unique_ptr<boost::thread> thread_ = nullptr;
    };

    /**
     * @brief Raised by a thread when it was interrupted by one if its state callbacks
     * or the termination flag was set.
     */
    struct thread_interruped : std::exception
    {
    public:
        thread_interruped(std::string t_thread_id, std::string t_why);

        [[nodiscard]] char const* what() const override;

        std::string id;
        std::string why;

    private:
        std::string what_;
    };

    using thread_registry_t = std::map<std::string, std::shared_ptr<managed_thread> >;

    /**
     * @brief Checks the state of the the current thread. Serves as a checkpoint for
     * terminated / paused threads to enter their appropriate states and should be
     * called regularly.
     */
    void check_thread_state();

    /**
     * @brief Registers a new thread into a global registry providing access by name.
     *
     * @param id The id to access the thread by.
     * @param target The function to execute when the thread is started later.
     *
     * @remark The thread does not automatically start when it is registered.
     * @remark If a thread with the id is already active, a loa_error is thrown.
     */
    void register_thread(const std::string& id, std::function<void()> target);

    /**
     * @brief Gets the data of a specific thread based on it's id.
     *
     * @param id The name of the thread to get the info of.
     *
     * @return A shared ptr to the thread info, or nullptr if the id is invalid.
     */
    [[nodiscard]] std::shared_ptr<managed_thread> get_thread(const std::string& id);

    /**
     * @brief Gets the thread from which this function was called.
     */
    [[nodiscard]] std::shared_ptr<managed_thread> get_this_thread();

    /**
     * @brief Retrieves all threads from the global registry.
     *
     * @return A map containing the name of each thread and it's data.
     */
    [[nodiscard]] thread_registry_t& get_all_threads();

    /**
     * @brief Pauses all threads in the global registry.
     *
     * @param state The state to set the threads to.
     * @param exclude a list of threads to exclude from the pausing.
     */
    void set_all_threads_state(managed_thread::ThreadState state,
                               const std::vector<std::string>& exclude = {});
}
