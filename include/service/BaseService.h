#ifndef BASESERVICE_H
#define BASESERVICE_H

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "service/IService.h"
#include "logging/HasLogger.h"

namespace PiAlarm::service {

    /**
     * @class BaseService
     * @brief Abstract base class for services in the PiAlarm application.
     *
     * This class implements the basic functionality for running, stopping, pausing, and resuming services.
     */
    class BaseService : public IService, public logging::HasLogger {
        std::atomic<bool> running_;  ///< Indicates if the service is currently running
        std::atomic<bool> paused_;   ///< Indicates if the service is currently paused
        std::thread workerThread_;   ///< Thread for running the service
        std::mutex mutex_;           ///< Mutex for synchronizing access to the service state
        std::condition_variable cv_; ///< Condition variable for managing pause/resume

    public:

        /**
         * @brief Constructor for BaseService.
         *
         * Initializes the service with a given name and sets the initial state to not running and not paused.
         * @param serviceName The name of the service.
         */
        BaseService(const std::string& serviceName);

        /**
         * @brief Destructor for BaseService.
         *
         * Stops the service if it is running.
         */
        virtual ~BaseService() override;

        /**
         * @brief Starts the service.
         *
         * This method begins the service's operation in a separate thread.
         * If the service is already running, this method does nothing.
         */
        void start() override;

        /**
         * @brief Stops the service.
         *
         * This method stops the service and joins the worker thread.
         * If the service is not running, this method does nothing.
         */
        void stop() override;

        /**
         * @brief Pauses the service.
         *
         * This method pauses the service's operation. The service can be resumed later.
         * If the service is not running, this method does nothing.
         */
        void pause() override;

        /**
         * @brief Resumes the service.
         *
         * This method resumes the service's operation if it is paused.
         * If the service is not running, this method does nothing.
         */
        void resume() override;

        /**
         * @brief Checks if the service is currently running.
         * @return true if the service is running, false otherwise.
         */
        [[nodiscard]]
        bool isRunning() const override;

        /**
         * @brief Checks if the service is currently paused.
         * @return true if the service is paused, false otherwise.
         */
        [[nodiscard]]
        bool isPaused() const override;

    protected:

        /**
         * @brief Method to be implemented by derived classes for service-specific functionality.
         *
         * This method is called in the worker thread and should contain the main logic of the service.
         * It is called repeatedly until the service is stopped.
         */
        virtual void update() = 0;

        /**
         * @brief Waits before the next update cycle.
         *
         * This method is called after each call to update() and is responsible for introducing a delay
         * between cycles. The default implementation uses std::this_thread::sleep_for with the value
         * returned by updateIntervalMs(). Derived classes may override this method to implement
         * more precise or adaptive waiting strategies, such as using std::this_thread::sleep_until
         * or event-based synchronization.
         *
         * @note Override this method if you require custom control over the timing between update cycles.
         */
        virtual void waitNextCycle();

        /**
         * @brief Returns the update interval for the service.
         *
         * This method provides a fixed time interval used by the default implementation of waitNextCycle().
         * Derived classes may override this method to specify a different update frequency.
         *
         * @note Override this method only if you are using the default sleep_for-based waitNextCycle().
         *
         * @warning If you override waitNextCycle(), this value may not be used.
         *
         * @return std::chrono::milliseconds Interval in milliseconds between update cycles. Default is 1000 ms (1 second).
         */
        [[nodiscard]]
        virtual inline std::chrono::milliseconds updateInterval() const {
            return std::chrono::milliseconds{1000};
        }

    private:
        /**
         * @brief The main loop of the service.
         *
         * This method runs in a separate thread and repeatedly calls the update method
         * while the service is running. It handles pausing and resuming the service.
         * Derived classes can override this to customize their execution logic.
         */
        virtual void run();

    };

} // namespace PiAlarm::service

#endif //BASESERVICE_H
