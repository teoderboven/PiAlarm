#pragma once

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "IService.h"
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

    protected:
        /**
         * @brief Method called in the worker thread just before entering the main loop.
         * Derived classes can override this to perform specific initialization (e.g., hardware setup).
         * If it returns false, the service will stop immediately.
         * @return true if initialization succeeded, false otherwise.
         */
        virtual bool onStart() { return true; }

        /**
         * @brief Method called just before the service loop terminates.
         * Derived classes can override this to release resources or put hardware to sleep.
         */
        virtual void onStop() {}

        /**
         * @brief Method to be implemented by derived classes for service-specific functionality.
         *
         * This method is called in the worker thread and should contain the main logic of the service.
         * It is called repeatedly until the service is stopped.
         */
        virtual void process() = 0;

        /**
         * @brief Waits before the next update cycle.
         *
         * This method is called after each call to process() and is responsible for introducing a delay
         * between cycles. The default implementation uses interruptibleSleepFor with the value
         * returned by updateInterval(). Derived classes may override this method to implement
         * more precise or adaptive waiting strategies, such as using interruptibleSleepUntil()
         * or event-based synchronization.
         *
         * @note Override this method if you require custom control over the timing between update cycles.
         * @warning Please use the given methods interruptibleSleepFor() or interruptibleSleepUntil()
         * to ensure that the service can be stopped gracefully during the wait.
         */
        virtual void waitNextCycle();

        /**
         * @brief Returns the update interval for the service.
         *
         * This method provides a fixed time interval used by the default implementation of waitNextCycle().
         * Derived classes may override this method to specify a different process frequency.
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

        /**
         * @brief Performs an interruptible sleep for a specified duration.
         *
         * This method uses the internal condition variable to wait, allowing the wait
         * to be interrupted if the service is stopped.
         * @param duration The duration to wait for.
         * @return true if the wait completed (duration elapsed or notified), false if the service was stopped.
         */
        bool interruptibleSleepFor(std::chrono::milliseconds duration);

        /**
         * @brief Performs an interruptible sleep until a specified time point.
         *
         * This method uses the internal condition variable to wait, allowing the wait
         * to be interrupted if the service is stopped.
         * @param time_point The time point to wait until.
         * @return true if the wait completed (time point reached or notified), false if the service was stopped.
         */
        bool interruptibleSleepUntil(std::chrono::system_clock::time_point time_point);

    public:

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

    private:
        /**
         * @brief The main execution loop of the service.
         *
         * This method runs in a dedicated thread and orchestrates the service life cycle
         * by chaining the initialization, the process loop, and the cleanup.
         */
        void run();

        /**
         * @brief Executes the initialization phase of the service.
         *
         * This method wraps the call to onStart() within an exception handling block
         * to prevent an application crash.
         * @return true if initialization succeeded, false otherwise.
         */
        bool executeStart();

        /**
         * @brief Executes a single cycle of the update loop.
         *
         * This method handles passive waiting related to pause or stop states,
         * calls the process() method, and applies the delay between cycles.
         * @return true if the service should continue running, false if it should stop.
         */
        bool executeCycle();

        /**
         * @brief Executes the cleanup and stop phase of the service.
         *
         * This method calls onStop() safely by intercepting all potential exceptions
         * to guarantee a clean termination of the worker thread.
         */
        void executeStop();
    };

} // namespace PiAlarm::service