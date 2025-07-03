#ifndef OBSERVER_H
#define OBSERVER_H

/**
 * @namespace PiAlarm::common
 * @brief Namespace for common components in the PiAlarm application.
 *
 * This namespace contains common components and utilities used across the PiAlarm application.
 */
namespace PiAlarm::common {

    /**
     * @interface Observer
     * @brief Interface for the observer in the Observer design pattern.
     *
     * This interface defines the methods that any observer must implement
     * to receive updates from the subject it is observing.
     */
    class Observer {
    public:

        /**
         * Updates the observer with new data.
         *
         * This method is called by the subject to notify the observer
         * that there is new data available or a state change has occurred.
         */
        virtual void update() = 0;

        /**
         * Virtual destructor for the observer interface.
         * Ensures proper cleanup of derived classes.
         */
        virtual ~Observer() = default;
    };

} // namespace PiAlarm::common

#endif //OBSERVER_H
