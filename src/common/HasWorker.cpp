#include "HasWorker.h"

namespace PiAlarm::common {

    HasWorker::HasWorker(const std::string& workerName)
        : HasLogger{workerName}, running_{false}, paused_{false}
    {}

    HasWorker::~HasWorker() {
        stopWorker();
    }

    void HasWorker::runWorker() {
        if (!executeWorkerStart()) {
            running_ = false;
            return;
        }

        while (running_) {
            if (!executeWorkerCycle()) {
                break;
            }
        }

        executeWorkerStop();
    }

    bool HasWorker::executeWorkerStart() {
        try {
            if (!onWorkerStart()) {
                logger().error("Worker initialization failed. Stopping worker.");
                return false;
            }
            return true;
        }
        catch (const std::exception& e) {
            logger().critical("Fatal exception during onWorkerStart: " + std::string(e.what()));
            return false;
        }
        catch (...) {
            logger().critical("Unknown fatal exception during onWorkerStart.");
            return false;
        }
    }

    bool HasWorker::executeWorkerCycle() {
        try {
            std::unique_lock lock{mutex_};
            cv_.wait(lock, [this]() { return !paused_ || !running_; });

            if (!running_) {
                return false;
            }
            lock.unlock();

            workerProcess();

            // Check running_ again in case stopWorker() was called during workerProcess()
            if (!running_) {
                return false;
            }

            workerWaitNextCycle();
        }
        catch (const std::exception& e) {
            logger().error("Exception caught during worker process: " + std::string(e.what()));
            std::this_thread::sleep_for(std::chrono::milliseconds{500});
        }
        catch (...) {
            logger().error("Unknown exception caught during worker process.");
            std::this_thread::sleep_for(std::chrono::milliseconds{500});
        }

        return true;
    }

    void HasWorker::executeWorkerStop() {
        try {
            onWorkerStop();
        }
        catch (const std::exception& e) {
            logger().error("Exception caught during onWorkerStop: " + std::string(e.what()));
        }
        catch (...) {
            logger().error("Unknown exception caught during onWorkerStop.");
        }
    }

    bool HasWorker::interruptibleSleepFor(std::chrono::milliseconds duration) {
        std::unique_lock lock{mutex_};
        cv_.wait_for(lock, duration, [this]() { return !running_; });
        return running_; // Return true if still running, false if stopped
    }

    bool HasWorker::interruptibleSleepUntil(std::chrono::system_clock::time_point time_point) {
        std::unique_lock lock{mutex_};
        cv_.wait_until(lock, time_point, [this]() { return !running_; });
        return running_; // Return true if still running, false if stopped
    }

    void HasWorker::workerWaitNextCycle() {
        interruptibleSleepFor(workerUpdateInterval());
    }

    void HasWorker::startWorker() {
        if (running_) return;

        running_ = true;
        workerThread_ = std::thread([this]() { runWorker(); });

        logger().info("Worker started");
    }

    void HasWorker::stopWorker() {
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
            logger().info("Worker stopped");
        } else {
            logger().info("Worker was already stopped");
        }
    }

    void HasWorker::pauseWorker() {
        if (running_) {
            std::lock_guard lock{mutex_};
            paused_ = true;
        }

        logger().info("Worker paused");
    }

    void HasWorker::resumeWorker() {
        {
            std::lock_guard lock{mutex_};
            paused_ = false;
        }
        cv_.notify_all();

        logger().info("Worker resumed");
    }

    bool HasWorker::isWorkerRunning() const {
        return running_.load();
    }

    bool HasWorker::isWorkerPaused() const {
        return paused_.load();
    }

} // namespace PiAlarm::common