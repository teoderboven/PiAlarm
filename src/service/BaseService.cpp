#include "service/BaseService.h"

namespace PiAlarm::service {

    BaseService::BaseService(const std::string& serviceName)
        : HasLogger{serviceName}, running_{false}, paused_{false}
    {}

    BaseService::~BaseService() {
        stop();
    }

    void BaseService::run() {
        while (running_) {
            std::unique_lock lock{mutex_};
            cv_.wait(lock, [this]() { return !paused_ || !running_; });

            if (!running_) break;
            lock.unlock();

            update();

            waitNextCycle();
        }
    }

    void BaseService::waitNextCycle() {
        std::this_thread::sleep_for(updateInterval());
    }

    void BaseService::start() {
        if (running_) return;

        running_ = true;
        workerThread_ = std::thread([this]() { run(); });

        logger().info("Service started");
    }

    void BaseService::stop() {
        {
            std::lock_guard lock{mutex_};
            running_ = false;
            paused_ = false;
        }
        cv_.notify_all();

        if (workerThread_.joinable() && std::this_thread::get_id() != workerThread_.get_id()) {
            workerThread_.join();
        }

        logger().info("Service stopped");
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
