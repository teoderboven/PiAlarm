#pragma once

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <string>

#include "logging/HasLogger.h"

namespace PiAlarm::common {

    /**
     * @class HasWorker
     * @brief Base class for objects that manage a worker thread.
     *
     * This class provides the core functionality for running, stopping, pausing, and resuming a worker thread.
     * Derived classes must implement the workerProcess() method to define the thread's main logic.
     */
    class HasWorker : public logging::HasLogger {
        std::atomic<bool> running_;  ///< Indicates if the worker is currently running
        std::atomic<bool> paused_;   ///< Indicates if the worker is currently paused
        std::thread workerThread_;   ///< Thread for running the worker
        std::mutex mutex_;           ///< Mutex for synchronizing access to the worker state
        std::condition_variable cv_; ///< Condition variable for managing pause/resume

    protected:
        /**
         * @brief Constructor for HasWorker.
         *
         * Initializes the worker with a given name and sets the initial state to not running and not paused.
         * @param workerName The name of the worker.
         */
        explicit HasWorker(const std::string& workerName);

        /**
         * @brief Destructor for HasWorker.
         *
         * Stops the worker if it is running.
         */
        ~HasWorker() override;

        /**
         * @brief Method called in the worker thread just before entering the main loop.
         * Derived classes can override this to perform specific initialization (e.g., hardware setup).
         * If it returns false, the worker will stop immediately.
         * @return true if initialization succeeded, false otherwise.
         */
        virtual bool onWorkerStart() { return true; }

        /**
         * @brief Method called just before the worker loop terminates.
         * Derived classes can override this to release resources or put hardware to sleep.
         */
        virtual void onWorkerStop() {}

        /**
         * @brief Method to be implemented by derived classes for worker-specific functionality.
         *
         * This method is called in the worker thread and should contain the main logic of the worker.
         * It is called repeatedly until the worker is stopped.
         */
        virtual void workerProcess() = 0;

        /**
         * @brief Waits before the next update cycle.
         *
         * This method is called after each call to workerProcess() and is responsible for introducing a delay
         * between cycles. The default implementation uses interruptibleSleepFor with the value
         * returned by workerUpdateInterval(). Derived classes may override this method to implement
         * more precise or adaptive waiting strategies, such as using interruptibleSleepUntil()
         * or event-based synchronization.
         *
         * @note Override this method if you require custom control over the timing between update cycles.
         * @warning Please use the given methods interruptibleSleepFor() or interruptibleSleepUntil()
         * to ensure that the worker can be stopped gracefully during the wait.
         */
        virtual void workerWaitNextCycle();

        /**
         * @brief Returns the update interval for the worker.
         *
         * This method provides a fixed time interval used by the default implementation of workerWaitNextCycle().
         * Derived classes may override this method to specify a different process frequency.
         *
         * @note Override this method only if you are using the default sleep_for-based workerWaitNextCycle().
         *
         * @warning If you override workerWaitNextCycle(), this value may not be used.
         *
         * @return std::chrono::milliseconds Interval in milliseconds between update cycles. Default is 1000 ms (1 second).
         */
        [[nodiscard]]
        virtual std::chrono::milliseconds workerUpdateInterval() const {
            return std::chrono::milliseconds{1000};
        }

        /**
         * @brief Performs an interruptible sleep for a specified duration.
         *
         * This method uses the internal condition variable to wait, allowing the wait
         * to be interrupted if the worker is stopped.
         * @param duration The duration to wait for.
         * @return true if the wait completed (duration elapsed or notified), false if the worker was stopped.
         */
        bool interruptibleSleepFor(std::chrono::milliseconds duration);

        /**
         * @brief Performs an interruptible sleep until a specified time point.
         *
         * This method uses the internal condition variable to wait, allowing the wait
         * to be interrupted if the worker is stopped.
         * @param time_point The time point to wait until.
         * @return true if the wait completed (time point reached or notified), false if the worker was stopped.
         */
        bool interruptibleSleepUntil(std::chrono::system_clock::time_point time_point);

    public:
        /**
         * @brief Starts the worker.
         *
         * This method begins the worker's operation in a separate thread.
         * If the worker is already running, this method does nothing.
         */
        void startWorker();

        /**
         * @brief Stops the worker.
         *
         * This method stops the worker and joins the worker thread.
         * If the worker is not running, this method does nothing.
         */
        void stopWorker();

        /**
         * @brief Pauses the worker.
         *
         * This method pauses the worker's operation. The worker can be resumed later.
         * If the worker is not running, this method does nothing.
         */
        void pauseWorker();

        /**
         * @brief Resumes the worker.
         *
         * This method resumes the worker's operation if it is paused.
         * If the worker is not running, this method does nothing.
         */
        void resumeWorker();

        /**
         * @brief Checks if the worker is currently running.
         * @return true if the worker is running, false otherwise.
         */
        [[nodiscard]]
        bool isWorkerRunning() const;

        /**
         * @brief Checks if the worker is currently paused.
         * @return true if the worker is paused, false otherwise.
         */
        [[nodiscard]]
        bool isWorkerPaused() const;

    private:
        /**
         * @brief The main execution loop of the worker.
         *
         * This method runs in a dedicated thread and orchestrates the worker life cycle
         * by chaining the initialization, the process loop, and the cleanup.
         */
        void runWorker();

        /**
         * @brief Executes the initialization phase of the worker.
         *
         * This method wraps the call to onWorkerStart() within an exception handling block
         * to prevent an application crash.
         * @return true if initialization succeeded, false otherwise.
         */
        bool executeWorkerStart();

        /**
         * @brief Executes a single cycle of the update loop.
         *
         * This method handles passive waiting related to pause or stop states,
         * calls the workerProcess() method, and applies the delay between cycles.
         * @return true if the worker should continue running, false if it should stop.
         */
        bool executeWorkerCycle();

        /**
         * @brief Executes the cleanup and stop phase of the worker.
         *
         * This method calls onWorkerStop() safely by intercepting all potential exceptions
         * to guarantee a clean termination of the worker thread.
         */
        void executeWorkerStop();
    };

} // namespace PiAlarm::common