#ifndef PIALARM_INPUTMANAGER_H
#define PIALARM_INPUTMANAGER_H

#include <vector>
#include <chrono>
#include "hardware/GPIO.h"
#include "input/InputEvent.h"

/**
 * @namespace PiAlarm::input
 * @brief Contains classes and functions for managing input events in the PiAlarm application.
 *
 * This namespace includes the InputManager class, which handles input events from GPIO buttons.
 */
namespace PiAlarm::input {

    /**
     * @class InputManager
     * @brief Manages input events from GPIO buttons.
     *
     * This class handles the state of multiple buttons, detects button presses,
     * and generates input events for the application.
     */
    class InputManager {
    public:

        /**
         * @struct ButtonConfig
         * @brief Represents the configuration for a button.
         *
         * This struct contains the GPIO object, button type, and settings for auto-repeat events.
         * It is used to initialize buttons in the InputManager.
         */
        struct ButtonConfig {
            hardware::GPIO& gpio; ///< The GPIO object for the button
            ButtonId type; ///< The type of button (Main, Back, Left, Right). Used to identify the button in events.
            bool generateRepeats; ///< Whether to generate auto-repeat events for this button

            ButtonConfig(hardware::GPIO& gpio, ButtonId type, bool generateRepeats = false)
                : gpio{gpio}, type{type}, generateRepeats{generateRepeats} {}
        };

    private:

        /**
         * @struct ManagedButton
         * @brief Represents a button managed by the InputManager.
         *
         * This struct contains the GPIO object, button type, pressed state,
         * auto-repeat settings, and timestamps for press and repeat events.
         */
        struct ManagedButton {
            hardware::GPIO& gpio; ///< The GPIO object for the button
            ButtonId type; ///< The type of button (Main, Back, Left, Right).
            bool pressed = false; ///< True if the button is currently pressed, false otherwise
            bool generateRepeats; ///< Whether to generate auto-repeat events for this button
            std::chrono::steady_clock::time_point lastPressTime; ///< The last time the button was pressed
            std::chrono::steady_clock::time_point lastRepeatTime; ///< The last time the button was repeated for auto-repeat
            std::chrono::steady_clock::time_point lastEventTime; ///< The last time an event was generated for this button

            /**
             * @brief Constructs a Button from a ButtonMapping.
             *
             * This constructor initializes the button with its GPIO and type.
             *
             * @param mapping The ButtonMapping containing the GPIO and type for the button
             */
            explicit ManagedButton(const ButtonConfig& mapping)
                : gpio{mapping.gpio}, type{mapping.type}, generateRepeats{mapping.generateRepeats} {}
        };

        static constexpr auto REPEAT_DELAY = std::chrono::milliseconds(500);    ///< Delay before auto-repeat starts
        static constexpr auto REPEAT_INTERVAL = std::chrono::milliseconds(100); ///< Interval between auto-repeat events
        static constexpr auto DEBOUNCE_DURATION = std::chrono::milliseconds(10); ///< Debounce duration for button presses

        using now_type = std::chrono::steady_clock::time_point; ///< Alias for the current time point type

        std::vector<ManagedButton> buttons_; ///< List of buttons managed by the InputManager

    public:
        using EventList = std::vector<InputEvent>; ///< Alias for a list of input events

        /**
         * @brief Constructs an InputManager with a list of button configurations.
         *
         * This constructor initializes the InputManager with the provided button configurations,
         * setting up each button for input event handling.
         *
         * @param buttonsConfig A vector of ButtonConfig objects representing the buttons to manage
         */
        explicit InputManager(const std::vector<ButtonConfig>& buttonsConfig);

        /**
         * @brief Polls for input events from the GPIO buttons.
         *
         * This method checks the state of each button, reads any events that have occurred,
         * and generates input events based on the button states. It also handles auto-repeat
         * functionality for buttons that are held down.
         *
         * @return A vector of InputEvent objects representing the input events detected
         */
        EventList pollEvents();

    private:

        /**
         * @brief Reads the state of a button and generates input events.
         *
         * This method checks the GPIO state of a button, updates its pressed state,
         * and generates input events based on the button's current state.
         *
         * @param button The Button struct representing the button to read
         * @param events The vector to store generated input events
         * @param now The current time point for event timestamping
         */
        static void readButton(ManagedButton& button, EventList &events, now_type now);

        /**
         * @brief Generates auto-repeat events for a button that is currently pressed.
         *
         * This method checks if the button is pressed and if the time since the last press
         * exceeds the repeat delay and interval. If so, it generates a repeat event.
         *
         * @param button The Button struct representing the button to generate repeats for
         * @param events The vector to store generated input events
         * @param now The current time point for event timestamping
         */
        static void generateRepeats(ManagedButton& button, EventList &events, now_type now);

    };

} // namespace PiAlarm::input

#endif //PIALARM_INPUTMANAGER_H
