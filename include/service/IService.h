#ifndef ISERVICE_H
#define ISERVICE_H

/**
 * @namespace PiAlarm::service
 * @brief Namespace for services in the PiAlarm application.
 *
 * This namespace contains interfaces and classes that define the services used in the PiAlarm application.
 */
namespace PiAlarm::service {

    /**
     * @interface IService
     * @brief Interface for services in the PiAlarm application.
     *
     * This interface defines the basic operations that any service in the PiAlarm application must implement.
     * Services can be started, stopped, paused, and resumed.
     */
    class IService {
    public:

        /**
         * @brief Virtual destructor for the interface.
         * Ensures proper cleanup of derived classes.
         */
        virtual ~IService() = default;

        /**
         * @brief Starts the service.
         */
        virtual void start() = 0;

        /**
         * @brief Stops the service.
         */
        virtual void stop() = 0;

        /**
         * @brief Pauses the service.
         */
        virtual void pause() = 0;

        /**
         * @brief Resumes the service.
         */
        virtual void resume() = 0;

        /**
         * @brief Checks if the service is currently running.
         * @return true if the service is running, false otherwise.
         */
        [[nodiscard]]
        virtual bool isRunning() const = 0;

        /**
         * @brief Checks if the service is currently paused.
         * @return true if the service is paused, false otherwise.
         */
        [[nodiscard]]
        virtual bool isPaused() const = 0;

    };

} // namespace PiAlarm::service

#endif //ISERVICE_H
