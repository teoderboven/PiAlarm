#include <csignal>
#include <iostream>
#include <thread>

#include "TerminationService.h"

namespace PiAlarm::system {

void TerminationService::register_hook(std::function<void()> hook) {
    std::lock_guard lock{get_mutex()};
    get_hooks().push_back(std::move(hook));
}

void TerminationService::initialize() {
    struct sigaction action;
    action.sa_handler = handle_signal;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    // list of all common signals that terminate the program by default
    const int termination_signals[] = {
        SIGINT,   // terminal interrupt (Ctrl+C)
        SIGTERM,  // software termination request (OS shutdown)
        SIGHUP,   // controlling terminal closed (hangup)
        SIGQUIT   // terminal quit (Ctrl+\) with core dump
    };

    // register the unique sigaction structure for each signal
    for (int sig : termination_signals) {
        if (sigaction(sig, &action, nullptr) < 0) {
            std::cerr << "[TerminationService] Failed to register sigaction for signal: " << sig << "\n";
        }
    }

    // spawn the background worker thread to process hooks safely
    std::thread cleanup_thread{&TerminationService::cleanup_worker};
    cleanup_thread.detach();
}

void TerminationService::handle_signal(int signal) {
    (void)signal;
    get_signal_received().store(true);

    // notify the worker thread immediately from the signal context
    get_cv().notify_one();
}

void TerminationService::cleanup_worker() {
    std::unique_lock lock{get_mutex()};

    // passive wait until a signal is caught
    get_cv().wait(lock, [] { return get_signal_received().load(); });

    for (const auto& hook : get_hooks()) {
        if (hook) {
            try {
                hook();
            } catch (const std::exception& e) {
                std::cerr << "[TerminationService] Error executing hook: " << e.what() << "\n";
            }
        }
    }
}

std::vector<std::function<void()>>& TerminationService::get_hooks() {
    static std::vector<std::function<void()>> hooks;
    return hooks;
}

std::mutex& TerminationService::get_mutex() {
    static std::mutex mutex;
    return mutex;
}

std::condition_variable& TerminationService::get_cv() {
    static std::condition_variable cv;
    return cv;
}

std::atomic<bool>& TerminationService::get_signal_received() {
    static std::atomic<bool> signal_received{false};
    return signal_received;
}

} // namespace PiAlarm::system