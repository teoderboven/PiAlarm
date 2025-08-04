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
    public:
        /**
         * @enum GPIOMode
         * @brief Represents the mode of a GPIO line.
         *
         * This enum defines the possible modes for a GPIO line:
         * - NONE: No mode set
         * - INPUT: GPIO line is set as input
         * - OUTPUT: GPIO line is set as output
         */
        enum class GPIOMode : int8_t{
            NONE = -1, ///< No mode set
            INPUT = 0,  ///< GPIO line is set as input
            OUTPUT = 1  ///< GPIO line is set as output
        };

    private:
        struct gpiod_chip* chip_; ///< Pointer to the GPIO chip
        struct gpiod_line* line_; ///< Pointer to the GPIO line
        GPIOMode mode_ = GPIOMode::NONE; ///< Current mode of the GPIO line (input or output)

    public:

        static constexpr int LOW = 0; ///< Constant for low value (0)
        static constexpr int HIGH = 1; ///< Constant for high value (1)

        static constexpr auto CONSUMER = "PiAlarm"; ///< Constant for consumer name (used in GPIO requests)

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
        inline explicit GPIO(unsigned int lineNumber);

        /**
         * Default destructor for GPIO.
         * Cleans up the GPIO chip and line resources.
         */
        ~GPIO();

        GPIO(const GPIO&) = delete; ///< No copy constructor
        GPIO& operator=(const GPIO&) = delete; ///< No copy assignment operator
        GPIO(GPIO&&) = delete; ///< No move constructor
        GPIO& operator=(GPIO&&) = delete; ///< No move assignment operator

        /**
         * Gets the current mode of the GPIO line.
         * @return The current mode (GPIOMode::INPUT or GPIOMode::OUTPUT).
         */
        [[nodiscard]]
        inline GPIOMode getMode() const;

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
        void set(int value) const;

        /**
         * Gets the current value of the GPIO line.
         * @return The current value of the line (0 or 1).
         * @throws std::runtime_error if the line value cannot be read.
         */
        [[nodiscard]]
        int get() const;

        /**
         * Checks if the GPIO line is set to high (1).
         * @return true if the line is high, false otherwise.
         */
        [[nodiscard]]
        inline bool isHigh() const;

        /**
         * Checks if the GPIO line is set to low (0).
         * @return true if the line is low, false otherwise.
         */
        [[nodiscard]]
        inline bool isLow() const;

    };

    // Inline methods implementations

    GPIO::GPIO(unsigned int lineNumber) : GPIO{"gpiochip0", lineNumber}
    {}

    inline GPIO::GPIOMode GPIO::getMode() const {
        return mode_;
    }

    inline bool GPIO::isHigh() const {
        return get() == HIGH;
    }

    inline bool GPIO::isLow() const {
        return get() == LOW;
    }

} // namespace PiAlarm::hardware

#endif //GPIO_H
