#include "input/InputManager.h"

namespace PiAlarm::input {

    InputManager::InputManager(const std::vector<ButtonConfig>& buttonsConfig) {
        buttons_.reserve(buttonsConfig.size());

        for (const auto& buttonConfig : buttonsConfig) {
            buttonConfig.gpio.setInputWithEdgeDetection(hardware::GPIO::EdgeType::BOTH);

            buttons_.emplace_back(buttonConfig);
        }
    }

    InputManager::EventList InputManager::pollEvents() {
        const auto now = std::chrono::steady_clock::now();

        EventList events;

        // Read the state of each button and generate events
        for (auto& button : buttons_) {
            readButton(button, events, now); // Read the button state and generate press/release events

            if (button.generateRepeats)
                generateRepeats(button, events, now); // Generate auto-repeat events if the button is pressed
        }

        return events;
    }

    void InputManager::readButton(ManagedButton& button, EventList &events, now_type now) {
        while (button.gpio.waitForEvent(0)) { // while there are events | 0 for non-blocking
            hardware::GPIOEvent event = button.gpio.readEvent();
            bool pressed = (event.type == hardware::GPIOEvent::Type::RisingEdge);

            if (button.pressed == pressed) {
                // If the button state hasn't changed, skip generating an event
                continue;
            }

            button.pressed = pressed;

            if (now - button.lastEventTime < DEBOUNCE_DURATION) {
                // If the event is too close to the last event, skip it to avoid flooding
                continue;
            }

            if (pressed) {
                button.lastPressTime = now;
                button.lastRepeatTime = now;
            }
            button.lastEventTime = now;

            events.push_back({ button.type, pressed });
        }
    }

    void InputManager::generateRepeats(ManagedButton& button, EventList &events, now_type now) {
        if (button.pressed) {
            if (now - button.lastPressTime >= REPEAT_DELAY &&
                now - button.lastRepeatTime >= REPEAT_INTERVAL)
            {
                events.push_back({ button.type, true }); // repeat event
                button.lastRepeatTime = now;
            }
        }
    }

} // namespace PiAlarm::input
