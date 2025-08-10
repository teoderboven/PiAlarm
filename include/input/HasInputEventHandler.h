#ifndef PIALARM_HASINPUTEVENTHANDLER_H
#define PIALARM_HASINPUTEVENTHANDLER_H

#include "input/InputEvent.h"

namespace PiAlarm::input {

    /**
     * @interface HasInputEventHandler
     * @brief Interface for components that can handle user input events.
     *
     * This interface defines a method for handling input events, allowing classes to respond to user inputs.
     */
    class HasInputEventHandler {
        public:

        /**
         * @brief Virtual destructor for HasInputEventHandler.
         * Ensures proper cleanup of derived classes.
         */
        virtual ~HasInputEventHandler() = default;

        /**
         * @brief Handles an input event.
         * This method should be implemented by derived classes to define how they respond to input events.
         *
         * @param event The input event to handle.
         */
        virtual void handleInputEvent(const InputEvent& event) = 0;

    };

} // namespace PiAlarm::input

#endif //PIALARM_HASINPUTEVENTHANDLER_H
