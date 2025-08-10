#ifndef PIALARM_INPUTEVENT_H
#define PIALARM_INPUTEVENT_H

namespace PiAlarm::input {

    /**
     * @enum ButtonId
     * @brief Represents the ID of a button.
     *
     * This enum defines the different buttons that can be used in the input system.
     * It is used to identify which button was pressed or released in an InputEvent.
     */
    enum class ButtonId {
        Main, ///< The main button, typically used for primary actions
        Back, ///< The back button, typically used to navigate back in the UI
        Previous, ///< The previous button, typically used for navigation or selection
        Next ///< The next button, typically used for navigation or selection
    };

    /**
     * @struct InputEvent
     * @brief Represents an input event for a button.
     *
     * This struct encapsulates the button ID and its state when an input event occurs.
     * It can be used to communicate button events in the application.
     */
    struct InputEvent {
        ButtonId button; ///< The ID of the button that triggered the event
        bool pressed; ///< True if the button was pressed, false if it was released
    };

} // namespace PiAlarm::input

#endif //PIALARM_INPUTEVENT_H
