#include "BaseService.h"

namespace PiAlarm::service {

    BaseService::BaseService(const std::string& serviceName)
        : HasLogger{serviceName}, running_{false}, paused_{false}
    {}

    BaseService::~BaseService() {
        stop();
    }

    void BaseService::run() {
        if (!executeStart()) {
            running_ = false;
            return;
        }

        while (running_) {
            if (!executeCycle()) {
                break;
            }
        }

        executeStop();
    }

    bool BaseService::executeStart() {
        try {
            if (!onStart()) {
                logger().error("Service initialization failed. Stopping service.");
                return false;
            }
            return true;
        }
        catch (const std::exception& e) {
            logger().critical("Fatal exception during onStart: " + std::string(e.what()));
            return false;
        }
        catch (...) {
            logger().critical("Unknown fatal exception during onStart.");
            return false;
        }
    }

    bool BaseService::executeCycle() {
        try {
            std::unique_lock lock{mutex_};
            cv_.wait(lock, [this]() { return !paused_ || !running_; });

            if (!running_) {
                return false;
            }
            lock.unlock();

            process();

            // Check running_ again in case stop() was called during process()
            if (!running_) {
                return false;
            }

            waitNextCycle();
        }
        catch (const std::exception& e) {
            logger().error("Exception caught during service process: " + std::string(e.what()));
            std::this_thread::sleep_for(std::chrono::milliseconds{500});
        }
        catch (...) {
            logger().error("Unknown exception caught during service process.");
            std::this_thread::sleep_for(std::chrono::milliseconds{500});
        }

        return true;
    }

    void BaseService::executeStop() {
        try {
            onStop();
        }
        catch (const std::exception& e) {
            logger().error("Exception caught during onStop: " + std::string(e.what()));
        }
        catch (...) {
            logger().error("Unknown exception caught during onStop.");
        }
    }

    bool BaseService::interruptibleSleepFor(std::chrono::milliseconds duration) {
        std::unique_lock lock{mutex_};
        cv_.wait_for(lock, duration, [this]() { return !running_; });
        return running_; // Return true if still running, false if stopped
    }

    bool BaseService::interruptibleSleepUntil(std::chrono::system_clock::time_point time_point) {
        std::unique_lock lock{mutex_};
        cv_.wait_until(lock, time_point, [this]() { return !running_; });
        return running_; // Return true if still running, false if stopped
    }

    void BaseService::waitNextCycle() {
        interruptibleSleepFor(updateInterval());
    }

    void BaseService::start() {
        if (running_) return;

        running_ = true;
        workerThread_ = std::thread([this]() { run(); });

        logger().info("Service started");
    }

    void BaseService::stop() {
        bool previouslyRunning;
        {
            std::lock_guard lock{mutex_};
            previouslyRunning = running_;
            running_ = false;
            paused_ = false;
        }
        cv_.notify_all();

        if (workerThread_.joinable() && std::this_thread::get_id() != workerThread_.get_id()) {
            workerThread_.join();
        }

        if (previouslyRunning) {
            logger().info("Service stopped");
        } else {
            logger().info("Service was already stopped");
        }
    }

    void BaseService::pause() {
        if (running_) {
            std::lock_guard lock{mutex_};
            paused_ = true;
        }

        logger().info("Service paused");
    }

    void BaseService::resume() {
        {
            std::lock_guard lock{mutex_};
            paused_ = false;
        }
        cv_.notify_all();

        logger().info("Service resumed");
    }

    bool BaseService::isRunning() const {
        return running_.load();
    }

    bool BaseService::isPaused() const {
        return paused_.load();
    }

} // namespace PiAlarm::service