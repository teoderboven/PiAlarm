#ifndef GPIO_H
#define GPIO_H

#include <gpiod.h>
#include <string>

/**
 * @namespace PiAlarm::hardware
 * @brief Contains hardware-related classes and functions for the PiAlarm application.
 *
 * This namespace includes classes for interacting with hardware components such as GPIO pins.
 */
namespace PiAlarm::hardware {

    /**
     * @class GPIO
     * @brief Represents a GPIO pin on a Raspberry Pi.
     *
     * This class provides methods to interact with a specific GPIO pin,
     * allowing it to be set as input or output, and to read or write values.
     *
     * GPIO stands for General Purpose Input/Output,
     * and is used to control or read the state of pins on a Raspberry Pi.
     */
    class GPIO {
        struct gpiod_chip* chip_; ///< Pointer to the GPIO chip
        struct gpiod_line* line_; ///< Pointer to the GPIO line

    public:

        static constexpr int LOW = 0; ///< Constant for low value (0)
        static constexpr int HIGH = 1; ///< Constant for high value (1)

        /**
         * Constructs a GPIO object for a specific chip and line number.
         * @param chipName The name of the GPIO chip (e.g., "gpiochip0").
         * @param lineNumber The line number on the chip to interact with.
         * @throws std::runtime_error if the chip or line cannot be opened.
         */
        GPIO(const std::string& chipName, unsigned int lineNumber);

        /**
         * Constructs a GPIO object for a specific line number on the default chip ("gpiochip0").
         * @param lineNumber The line number to interact with.
         * @throws std::runtime_error if the default chip or line cannot be opened.
         */
        explicit GPIO(unsigned int lineNumber);

        /**
         * Default destructor for GPIO.
         * Cleans up the GPIO chip and line resources.
         */
        ~GPIO();

        /**
         * Sets the GPIO line as an output with an initial value.
         * @param initialValue The initial value to set for the output (0 or 1).
         * @throws std::runtime_error if the line cannot be set as output.
         */
        void setOutput(int initialValue = 0);

        /**
         * Sets the GPIO line as an input.
         * @throws std::runtime_error if the line cannot be set as input.
         */
        void setInput();

        /**
         * Sets the GPIO line to a specific value.
         * @param value The value to set (0 or 1).
         * @throws std::runtime_error if the line value cannot be set.
         */
        void set(int value);

        /**
         * Gets the current value of the GPIO line.
         * @return The current value of the line (0 or 1).
         * @throws std::runtime_error if the line value cannot be read.
         */
        int get();

    };

} // namespace PiAlarm::hardware

#endif //GPIO_H
