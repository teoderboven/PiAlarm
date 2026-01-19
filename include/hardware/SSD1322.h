#ifndef SSD1322_H
#define SSD1322_H

#ifdef RASPBERRY_PI

#include <cstdint>
#include <array>

#include "hardware/GPIO.h"
#include "hardware/SPI.h"

// SSD1322 Command Definitions
// copied from https://github.com/venice1200/SSD1322_for_Adafruit_GFX/blob/v0.1.2/SSD1322_for_Adafruit_GFX.h
#define SSD1322_SETCOLUMN 0x15
#define SSD1322_ENWRITEDATA 0x5C               // Enable Write Data
#define SSD1322_SETROW 0x75
#define SSD1322_SEGREMAP 0xA0
#define SSD1322_SETSTARTLINE 0xA1
#define SSD1322_SETDISPLAYOFFSET 0xA2
#define SSD1322_DISPLAYALLOFF 0xA4              // All Pixel OFF in GS level 0
#define SSD1322_DISPLAYALLON 0xA5               // All Pixel ON in GS level 15
#define SSD1322_NORMALDISPLAY 0xA6
#define SSD1322_INVERTDISPLAY 0xA7
#define SSD1322_ENPARTDISPLAY 0xA8
#define SSD1322_EXITPARTDISPLAY 0xA9
#define SSD1322_SETMULTIPLEX 0xCA
#define SSD1322_FUNCSEL 0xAB
#define SSD1322_DISPLAYOFF 0xAE
#define SSD1322_DISPLAYON 0xAF
#define SSD1322_PHASELEN 0xB1
#define SSD1322_DISPLAYCLK 0xB3
#define SSD1322_DISPLAYENHA 0xB4
#define SSD1322_SETGPIO 0xB5
#define SSD1322_PRECHARGE2 0xB6
#define SSD1322_GRAYTABLE 0xB8
#define SSD1322_PRECHARGE 0xBB
#define SSD1322_SETVCOM 0xBE
#define SSD1322_SETCONTRAST 0xC1
#define SSD1322_MASTERCONTRAST 0xC7
#define SSD1322_DISPLAYENHB 0xD1
#define SSD1322_FUNCSELB 0xD5
#define SSD1322_CMDLOCK 0xFD

namespace PiAlarm::hardware {

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
        SPI& spi_; ///< Reference to the SPI interface used for communication
        GPIO& dcPin_; ///< Data/Command pin for the SSD1322 display
        GPIO& resetPin_; ///< Reset pin for the SSD1322 display

    public:
        static constexpr size_t DISPLAY_WIDTH = 256; ///< Width of the SSD1322 display in pixels
        static constexpr size_t DISPLAY_HEIGHT = 64; ///< Height of the SSD1322 display in pixels

    private:
        /*
         * COLUMN note:
         * The SSD1322 ram can contain more columns than the physical display,
         * so the current column range is from 0x1C to 0x5B to be centered in the display.
         */
        static constexpr uint8_t COLUMN_START = 0x1C; ///< Start column for the display buffer (28 in decimal)
        static constexpr uint8_t COLUMN_END = 0x5B; ///< End column for the display buffer (91 in decimal, 256 pixels / 4 bits per pixel = 64 columns, so 0x5B = 28 + 64 - 1 = 91)
        static constexpr uint8_t ROW_START = 0x00; ///< Start row for the display buffer
        static constexpr uint8_t ROW_END = 0x3F; ///< End row for the display buffer (63 in decimal)

    public:
        using CommandByte = uint8_t; ///< Type alias for command byte
        using DataByte = uint8_t; ///< Type alias for data byte

        /**
         * Constructs an SSD1322 object with the specified SPI and GPIO pins.
         * @param spi Reference to the SPI interface used for communication.
         * @param dcPin Reference to the GPIO pin used for Data/Command selection.
         * @param resetPin Reference to the GPIO pin used for resetting the display.
         * @note The GPIO pins are configured as output by this constructor.
         * @note After creating the SSD1322 object, the `initialize()` method should be called to set up the display.
         */
        SSD1322(SPI& spi, GPIO& dcPin, GPIO& resetPin);

        /**
         * Initializes the SSD1322 display.
         * This method sends the necessary commands to configure the display.
         * It should be called after creating the SSD1322 object.
         */
        void initialize() const;

        /**
         * Resets the SSD1322 display.
         * This method sets the reset pin low for a short duration and then sets it high again.
         * It is typically called at the start of the initialization process.
         */
        void reset() const;

        /**
         * Sends a command to the SSD1322 display.
         * @param cmd The command byte to send.
         * @note The command is sent in command mode, which is set by the `setDCPinCommand()` method.
         */
        void sendCommand(CommandByte cmd) const;

        /**
         * Sends data to the SSD1322 display.
         * @param data The data byte to send.
         * @note The data is sent in data mode, which is set by the `setDCPinData()` method.
         */
        void sendData(DataByte data) const;

        /**
         * Sends an array of data bytes to the SSD1322 display.
         * @param data Pointer to the array of data bytes.
         * @param length The number of bytes to send.
         * @note The data is sent in data mode, which is set by the `setDCPinData()` method.
         */
        void sendData(const DataByte* data, size_t length) const;

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
        void flush(const uint8_t* buffer, size_t size) const;

        /**
         * Sets the contrast of the SSD1322 display.
         * @param contrast The contrast value to set (0-255).
         * @note This method sends a command to set the contrast level of the display.
         * @note Try to use values between 0x00 and Ox80 to avoid long-term damages to the display.
         * @note The recommended default value is 0x50.
         */
        void setContrast(uint8_t contrast) const;

        /**
         * Turns on all pixels on the SSD1322 display.
         * This method sends a command to turn on all pixels, making the display fully lit.
         * @note Call `setNormalDisplay()` to return to normal mode after this.
         */
        void allPixelsOn() const;

        /**
         * Turns off all pixels on the SSD1322 display.
         * This method sends a command to turn off all pixels, making the display fully dark.
         * @note Call `setNormalDisplay()` to return to normal mode after this.
         */
        void allPixelsOff() const;

        /**
         * Sets the display to normal mode (non-inverted).
         * This method sends a command to set the display to normal mode, where pixels are displayed as they are in the buffer.
         */
        void setNormalDisplay() const;

    private:

        /**
         * Sets the DC pin to command mode.
         * This method is used to indicate that the next byte sent is a command.
         */
        void setDCPinCommand() const;

        /**
         * Sets the DC pin to data mode.
         * This method is used to indicate that the next byte sent is data.
         */
        void setDCPinData() const;
    };

} // namespace PiAlarm::hardware

#endif // RASPBERRY_PI
#endif //SSD1322_H
