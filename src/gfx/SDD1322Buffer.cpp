#include "gfx/SDD1322Buffer.h"

namespace PiAlarm::gfx {

    SDD1322Buffer::SDD1322Buffer() {
        buffer_.fill(0x00); // Initialize the buffer to black
    }

    void SDD1322Buffer::setPixel(size_t x, size_t y, Pixel grayscale) {
        if (x>= BUFFER_PIXEL_WIDTH || y >= BUFFER_PIXEL_HEIGHT) return; // Out of bounds check

        auto gray = get4BitGrayscale(grayscale);
        size_t byteIndex = y * (BUFFER_PIXEL_WIDTH/2) + (x/2);

        if (x % 2 == 0) {
            // Even column, set the high nibble
            buffer_.at(byteIndex) = (buffer_[byteIndex] & 0b0000'1111) | (gray << 4);
        } else {
            // Odd column, set the low nibble
            buffer_.at(byteIndex) = (buffer_[byteIndex] & 0b1111'0000) | gray;
        }
    }

} // namespace PiAlarm::gfx
