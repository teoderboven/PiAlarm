#pragma once

#ifdef RASPBERRY_PI

#include <cstdint>

#include "GPIO.h"
#include "SPI.h"

namespace PiAlarm::hardware {

    // SSD1322 Datasheet: https://www.hpinfotech.ro/SSD1322.pdf
    // NHD-3.12-25664UCW2 Datasheet (the physical screen used): https://newhavendisplay.com/content/specs/NHD-3.12-25664UCW2.pdf

    // Not using the premade driver, causes it uses AdaFruit GFX library which is not compatible with the current project.
    /**
     * @class SSD1322
     * @brief Represents an SSD1322 OLED display controller.
     *
     * This class provides methods to initialize the SSD1322 display,
     * send commands and data, and clear the screen.
     *
     * The SSD1322 is a monochrome OLED display controller with a resolution of 256x64 pixels.
     *
     * Mostly based on the venice1200/SSD1322_for_Adafruit_GFX library.
     * (https://github.com/venice1200/SSD1322_for_Adafruit_GFX)
     *
     * Made with the help of ChatGPT and the SSD1322 datasheet.
     */
    class SSD1322 {
        SPI spi_; ///< The SPI interface used for communication
        GPIO dcPin_; ///< Data/Command pin for the SSD1322 display
        GPIO resetPin_; ///< Reset pin for the SSD1322 display

    public:
        using CommandByte = uint8_t; ///< Type alias for command byte
        using DataByte = uint8_t; ///< Type alias for data byte

        static constexpr size_t DISPLAY_WIDTH {256}; ///< Width of the SSD1322 display in pixels
        static constexpr size_t DISPLAY_HEIGHT {64}; ///< Height of the SSD1322 display in pixels

        /**
         * @brief Constructs an SSD1322 object, creating and taking ownership of SPI and GPIO resources.
         *
         * @param dcLineParams Configuration of the dcLine (data/command) GPIO pin.
         * @param resetLineParams Configuration of the reset GPIO pin.
         * @param spiChipSelect The SPI chip select (default is 0).
         * @param spiSpeed The speed of the SPI communication in Hz (default is 10 MHz).
         * @throws std::runtime_error if any hardware resource cannot be initialized.
         * @note The GPIO pins are configured as output by this constructor.
         * @note After creating the SSD1322 object, the `initialize()` method should be called to set up the display.
         */
        SSD1322(const GPIOConfig& dcLineParams,
                const GPIOConfig& resetLineParams,
                uint32_t spiChipSelect = 0,
                uint32_t spiSpeed = 10'000'000);

        /**
         * @brief Initializes the SSD1322 display.
         *
         * This method sends the necessary commands to configure the display.
         * It should be called after creating the SSD1322 object.
         *
         * @throw std::runtime_error if any hardware resource cannot be initialized.
         */
        void initialize();

        /**
         * @brief Resets the SSD1322 display.
         *
         * This method sets the reset pin low for a short duration and then sets it high again.
         * It is typically called at the start of the initialization process.
         */
        void reset();

        /**
         * @brief Sends a command to the SSD1322 display.
         *
         * @param cmd The command byte to send.
         * @note The command is sent in command mode, which is set by the `setDCPinCommand()` method.
         */
        void sendCommand(CommandByte cmd);

        /**
         * @brief Sends data to the SSD1322 display.
         *
         * @param data The data byte to send.
         * @note The data is sent in data mode, which is set by the `setDCPinData()` method.
         */
        void sendData(DataByte data);

        /**
         * @brief Sends an array of data bytes to the SSD1322 display.
         *
         * @param data Pointer to the array of data bytes.
         * @param length The number of bytes to send.
         * @note The data is sent in data mode, which is set by the `setDCPinData()` method.
         */
        void sendData(const DataByte* data, size_t length);

        /**
         * @brief Transfers a 4-bit grayscale framebuffer to the SSD1322 display.
         *
         * This method sets the drawing area (256x64 pixels), enables graphic data write mode,
         * and sends the entire framebuffer to the display.
         *
         * @param buffer Pointer to the framebuffer (must be 4 bits per pixel, packed: 2 pixels per byte).
         * @param size
         * @note The size of the framebuffer must match the defined BUFFER_WIDTH * BUFFER_HEIGHT (8192 for 64*128 bytes).
         */
        void flush(const uint8_t* buffer, size_t size);

        /**
         * @brief Sets the contrast of the SSD1322 display.
         * @param contrast The contrast value to set (0-255).
         * @note This method sends a command to set the contrast level of the display.
         * @note Try to use values between 0x00 and Ox80 to avoid long-term damages to the display.
         * @note The recommended default value is 0x50.
         */
        void setContrast(uint8_t contrast);

        /**
         * @brief Turns on all pixels on the SSD1322 display.
         *
         * This method sends a command to turn on all pixels, making the display fully lit.
         * @note Call `setNormalDisplay()` to return to normal mode after this.
         */
        void allPixelsOn();

        /**
         * @brief Turns off all pixels on the SSD1322 display.
         *
         * This method sends a command to turn off all pixels, making the display fully dark.
         * @note Call `setNormalDisplay()` to return to normal mode after this.
         */
        void allPixelsOff();

        /**
         * @brief Sets the display to normal mode (non-inverted).
         *
         * This method sends a command to set the display to normal mode, where pixels are displayed as they are in the buffer.
         */
        void setNormalDisplay();

    private:
        // SSD1322 Command Definitions
        // Adapted from https://github.com/venice1200/SSD1322_for_Adafruit_GFX/blob/v0.1.2/SSD1322_for_Adafruit_GFX.h
        static constexpr CommandByte        SETCOLUMN {0x15};
        static constexpr CommandByte      ENWRITEDATA {0x5C}; // Enable Write Data
        static constexpr CommandByte           SETROW {0x75};
        static constexpr CommandByte         SEGREMAP {0xA0};
        static constexpr CommandByte     SETSTARTLINE {0xA1};
        static constexpr CommandByte SETDISPLAYOFFSET {0xA2};
        static constexpr CommandByte    DISPLAYALLOFF {0xA4}; // All Pixel OFF in GS level 0
        static constexpr CommandByte     DISPLAYALLON {0xA5}; // All Pixel ON in GS level 15
        static constexpr CommandByte    NORMALDISPLAY {0xA6};
        static constexpr CommandByte    INVERTDISPLAY {0xA7};
        static constexpr CommandByte    ENPARTDISPLAY {0xA8};
        static constexpr CommandByte  EXITPARTDISPLAY {0xA9};
        static constexpr CommandByte     SETMULTIPLEX {0xCA};
        static constexpr CommandByte          FUNCSEL {0xAB};
        static constexpr CommandByte       DISPLAYOFF {0xAE};
        static constexpr CommandByte        DISPLAYON {0xAF};
        static constexpr CommandByte         PHASELEN {0xB1};
        static constexpr CommandByte       DISPLAYCLK {0xB3};
        static constexpr CommandByte      DISPLAYENHA {0xB4};
        static constexpr CommandByte          SETGPIO {0xB5};
        static constexpr CommandByte       PRECHARGE2 {0xB6};
        static constexpr CommandByte        GRAYTABLE {0xB8};
        static constexpr CommandByte        PRECHARGE {0xBB};
        static constexpr CommandByte          SETVCOM {0xBE};
        static constexpr CommandByte      SETCONTRAST {0xC1};
        static constexpr CommandByte   MASTERCONTRAST {0xC7};
        static constexpr CommandByte      DISPLAYENHB {0xD1};
        static constexpr CommandByte         FUNCSELB {0xD5};
        static constexpr CommandByte          CMDLOCK {0xFD};

        /*
         * COLUMN note:
         * The SSD1322 ram can contain more columns than the physical display,
         * so the current column range is from 0x1C to 0x5B to be centered in the display.
         */
        static constexpr uint8_t COLUMN_START {0x1C}; ///< Start column for the display buffer (28 in decimal)
        static constexpr uint8_t COLUMN_END {0x5B}; ///< End column for the display buffer (91 in decimal, 256 pixels / 4 bits per pixel = 64 columns, so 0x5B = 28 + 64 - 1 = 91)
        static constexpr uint8_t ROW_START {0x00}; ///< Start row for the display buffer
        static constexpr uint8_t ROW_END {0x3F}; ///< End row for the display buffer (63 in decimal)

        /**
         * @brief Sets the DC pin to command mode.
         *
         * This method is used to indicate that the next byte sent is a command.
         */
        void setDCPinCommand();

        /**
         * @brief Sets the DC pin to data mode.
         *
         * This method is used to indicate that the next byte sent is data.
         */
        void setDCPinData();
    };

} // namespace PiAlarm::hardware

#endif // RASPBERRY_PI