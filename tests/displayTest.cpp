#include "hardware/GPIO.h"
#include "hardware/SPI.h"
#include "hardware/SSD1322.h"
#include "gfx/SDD1322Buffer.h"
#include "gfx/Canvas.h"
#include "gfx/TrueTypeFont.h"

#include <iostream>

// This is a simple test program to demonstrate drawing text and bitmaps on an SSD1322 OLED display using the PiAlarm library.
int main() {
    using namespace PiAlarm;

    // Hardware setup
    // Adjust the GPIO and SPI pins according to your hardware configuration
    hardware::SPI oledSpi1{"/dev/spidev0.0", 10'000'000}; // SPI device for OLED display
    hardware::GPIO oledDcPin1{25}; // GPIO pin for Data/Command selection
    hardware::GPIO oledResetPin1{24}; // GPIO pin for reset

    hardware::SSD1322 oledDisplay{oledSpi1, oledDcPin1, oledResetPin1}; // SSD1322 OLED display
    oledDisplay.initialize(); // Initialize the OLED display

    // GFX

    int fontSize = 12;
    gfx::TrueTypeFont font("assets/fonts/Roboto-Regular.ttf", fontSize);

    gfx::Canvas canvas{std::make_unique<gfx::SDD1322Buffer>(), gfx::Canvas::DrawMode::IgnoreBlack}; // Create a canvas with the SSD1322 buffer

    // Using Bitmaps to draw lines
    gfx::Bitmap bitmap(256, 1);
    bitmap.fill(0x10); // First level of white

    gfx::Bitmap bitmap2(1, 64);
    bitmap2.fill(0x10);

    canvas.drawBitmap(0, fontSize/2, bitmap);
    canvas.drawBitmap(0, 32, bitmap);
    canvas.drawBitmap(0, 64 - fontSize/2, bitmap);
    canvas.drawBitmap(0, 0, bitmap2);
    canvas.drawBitmap(128, 0, bitmap2);
    canvas.drawBitmap(255, 0, bitmap2);

    // Draw text at every anchor positions
    using gfx::Canvas;
    // canvas.setDrawMode(Canvas::DrawMode::InvertAndDisplayAll);

    canvas.drawText(0, 0, "Top-left", font, Canvas::Anchor::TopLeft);
    canvas.drawText(0, canvas.getHeight() / 2, "Middle-left", font, Canvas::Anchor::MiddleLeft);
    canvas.drawText(0, canvas.getHeight() - 1, "Bottom-left", font, Canvas::Anchor::BottomLeft);

    canvas.drawText(canvas.getWidth() / 2, 0, "Top-center", font, Canvas::Anchor::TopCenter);
    canvas.drawText(canvas.getWidth() / 2, canvas.getHeight() / 2, "Center", font, Canvas::Anchor::Center);
    canvas.drawText(canvas.getWidth() / 2, canvas.getHeight() - 1, "Bottom-center", font, Canvas::Anchor::BottomCenter);

    canvas.drawText(canvas.getWidth() - 1, 0, "Top-right", font, Canvas::Anchor::TopRight);
    canvas.drawText(canvas.getWidth() - 1, canvas.getHeight() / 2, "Middle-right", font, Canvas::Anchor::MiddleRight);
    canvas.drawText(canvas.getWidth() - 1, canvas.getHeight() - 1, "Bottom-right", font, Canvas::Anchor::BottomRight);

    oledDisplay.flush(canvas.buffer().data(), canvas.buffer().size());

    std::cout << "Buffer wrote to the display." << std::endl;

    return 0;
}
