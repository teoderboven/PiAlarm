#ifndef PIALARM_TERMINATIONSERVICE_H
#define PIALARM_TERMINATIONSERVICE_H

#include <functional>
#include <vector>
#include <mutex>
#include <atomic>
#include <condition_variable>

/**
 * @namespace PiAlarm::system
 * @brief Namespace for system-level utilities and services in the PiAlarm application.
 *
 * This namespace contains classes and functions that manage system-level operations,
 * such as signal handling, shutdown procedures, and other OS-level interactions.
 */
namespace PiAlarm::system {

/**
 * @brief Thread-safe service responsible for catching OS termination signals and executing registered cleanup hooks.
 *
 * This service catches SIGINT, SIGTERM, SIGHUP and SIGQUIT signals asynchronously, then safely delegates
 * the execution of registered cleanup callbacks to a dedicated background worker thread,
 * avoiding undefined behaviors linked to async-signal contexts.
 */
class TerminationService {
public:
    // No constructor, class is static
    TerminationService() = delete;
    ~TerminationService() = delete;

    /**
     * @brief Registers a cleanup callback function to be executed upon receiving a termination signal.
     * @param hook A callable function or lambda expression containing cleanup logic.
     * @note You can use std::bind if you need to pass arguments to the callback.
     */
    static void register_hook(std::function<void()> hook);

    /**
     * @brief Initializes OS signal listeners and spawns the dedicated cleanup worker thread.
     */
    static void initialize();

private:
    /**
     * @brief Low-level async signal handler callback required by std::signal.
     * @param signal The received signal identifier (e.g., SIGINT, SIGTERM).
     */
    static void handle_signal(int signal);

    /**
     * @brief Dedicated worker loop executing the registered hooks inside a safe thread context.
     */
    static void cleanup_worker();

    /**
     * @brief Retrieves the static list of registered hooks using Meyers' Singleton pattern.
     * @return Reference to the internal vector of functions.
     */
    static std::vector<std::function<void()>>& get_hooks();

    /**
     * @brief Retrieves the static mutex used to protect hook registration and execution.
     * @return Reference to the internal mutex.
     */
    static std::mutex& get_mutex();

    /**
     * @brief Retrieves the condition variable used to wake up the cleanup worker thread.
     * @return Reference to the internal condition variable.
     */
    static std::condition_variable& get_cv();

    /**
     * @brief Retrieves the atomic flag tracking whether a termination signal has been captured.
     * @return Reference to the internal atomic boolean.
     */
    static std::atomic<bool>& get_signal_received();
};

} // namespace PiAlarm::system

#endif //PIALARM_TERMINATIONSERVICE_H
