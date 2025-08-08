#ifdef __linux__

#include "hardware/SSD1322.h"
#include <thread>
#include <chrono>
#include <cassert>

namespace PiAlarm::hardware {

    SSD1322::SSD1322(SPI& spi, GPIO& dcPin, GPIO& resetPin)
        : spi_{spi}, dcPin_{dcPin}, resetPin_{resetPin}
    {
        // Configure GPIO pins as output
        // DC pin is used to switch between command and data mode
        // Reset pin is used to reset the display
        dcPin_.setOutput(GPIO::LOW);
        resetPin_.setOutput(GPIO::HIGH);
    }

    void SSD1322::reset() const {
        resetPin_.set(GPIO::LOW);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        resetPin_.set(GPIO::HIGH);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    void SSD1322::setDCPinCommand() const {
        dcPin_.set(GPIO::LOW);
    }

    void SSD1322::setDCPinData() const {
        dcPin_.set(GPIO::HIGH);
    }

    void SSD1322::sendCommand(CommandByte cmd) const {
        setDCPinCommand();
        spi_.writeByte(cmd);
    }

    void SSD1322::sendData(DataByte data) const {
        setDCPinData();
        spi_.writeByte(data);
    }

    void SSD1322::sendData(const DataByte* data, size_t length) const {
        setDCPinData();
        spi_.writeData(data, length);
    }

    void SSD1322::flush(const uint8_t* buffer, size_t size) const {
        assert(size == DISPLAY_HEIGHT * (DISPLAY_WIDTH/2)); // Ensure the buffer size matches the expected dimensions

        // Set the drawing area to the full display size
        sendCommand(SSD1322_SETCOLUMN);
        sendData(COLUMN_START); // Start column
        sendData(COLUMN_END); // End column (256 pixels / 4 bits per pixel = 64 columns)

        sendCommand(SSD1322_SETROW);
        sendData(ROW_START); // Start row
        sendData(ROW_END); // End row (63)

        // Enable graphic data write mode
        sendCommand(SSD1322_ENWRITEDATA);

        // Send the framebuffer data
        sendData(buffer, size);
    }

    void SSD1322::initialize() const {
        reset();

        // commands documentation: https://www.crystalfontz.com/controllers/datasheet-viewer.php?id=427 (chapter 9 & 10)
        sendCommand(SSD1322_CMDLOCK);          // 0xFD - Command Lock
        sendData(0x12);                             // Unlock OLED driver IC

        sendCommand(SSD1322_DISPLAYOFF);       // 0xAE - Display OFF

        sendCommand(SSD1322_DISPLAYCLK);       // 0xB3 - Set Display Clock Divide Ratio/Oscillator Frequency
        sendData(0x91);                             // Divide by 2, ~80 FPS

        sendCommand(SSD1322_SETMULTIPLEX);     // 0xCA - Set Multiplex Ratio
        sendData(0x3F);                             // 1/64

        sendCommand(SSD1322_SETDISPLAYOFFSET); // 0xA2 - Set Display Offset
        sendData(0x00);                             // No offset

        sendCommand(SSD1322_SETSTARTLINE);     // 0xA1 - Set Display Start Line
        sendData(0x00);                             // Line 0

        sendCommand(SSD1322_SEGREMAP);         // 0xA0 - Set Remap and Dual COM Mode
        sendData(0x14);                             // Horizontal increment, nibble remap
        sendData(0x11);                             // Dual COM mode

        sendCommand(SSD1322_SETGPIO);          // 0xB5 - Set GPIO
        sendData(0x00);                             // Input disabled

        sendCommand(SSD1322_FUNCSEL);          // 0xAB - Function Selection
        sendData(0x01);                             // Enable internal VDD regulator

        sendCommand(SSD1322_DISPLAYENHA);      // 0xB4 - Display Enhancement A
        sendData(0xA0);                             // Enable external VSL
        sendData(0xFD);                             // Enhanced low GS display quality

        sendCommand(SSD1322_MASTERCONTRAST);   // 0xC7 - Master Contrast
        sendData(0x0F);                             // Maximum contrast

        sendCommand(SSD1322_PHASELEN);         // 0xB1 - Set Phase Length
        sendData(0xE2);                             // Phase 1: 5 DCLKs, Phase 2: 14 DCLKs

        sendCommand(SSD1322_DISPLAYENHB);      // 0xD1 - Display Enhancement B
        sendData(0xA2);
        sendData(0x20);

        sendCommand(SSD1322_PRECHARGE);        // 0xBB - Precharge Voltage
        sendData(0x1F);                             // 0.6 x VCC

        sendCommand(SSD1322_PRECHARGE2);       // 0xB6 - Second Precharge Period
        sendData(0x08);                             // 8 DCLKs

        sendCommand(SSD1322_SETVCOM);          // 0xBE - VCOMH Voltage
        sendData(0x07);                             // 0.86 x VCC

        setNormalDisplay(); // Set normal display mode

        sendCommand(SSD1322_EXITPARTDISPLAY);  // 0xA9 - Exit Partial Display mode

        setContrast(0x50); // Set default contrast

        sendCommand(SSD1322_DISPLAYON);        // 0xAF - Display ON
    }

    void SSD1322::setContrast(uint8_t contrast) const {
        sendCommand(SSD1322_SETCONTRAST);
        sendData(contrast);
    }

    void SSD1322::allPixelsOn() const {
        sendCommand(SSD1322_DISPLAYALLON);  // 0xA5
    }

    void SSD1322::allPixelsOff() const {
        sendCommand(SSD1322_DISPLAYALLOFF); // 0xA4
    }

    void SSD1322::setNormalDisplay() const {
        sendCommand(SSD1322_NORMALDISPLAY); // 0xA6
    }

} // namespace PiAlarm::hardware

#endif // __linux__
