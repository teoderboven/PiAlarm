#ifndef SDD1322BUFFER_H
#define SDD1322BUFFER_H

#include <array>

#include "gfx/IBuffer.h"

namespace PiAlarm::gfx {

    /**
     * @class SDD1322Buffer
     * @brief Represents a buffer for the SSD1322 display.
     *
     * This class implements the IBuffer interface and provides methods to manage
     * a framebuffer for the SSD1322 OLED display, which has a resolution of 256x64 pixels.
     */
    class SDD1322Buffer : public IBuffer {
    public:
        static constexpr size_t BUFFER_PIXEL_WIDTH = 256; ///< Width of the buffer in pixels
        static constexpr size_t BUFFER_PIXEL_HEIGHT = 64; ///< Height of the buffer in pixels

    private:
        using PixelPairByte = uint8_t; ///< Type alias for a byte representing 2 pixels (4 bits per pixel)
        using PixelGrayscale = uint8_t; ///< Type alias for a pixel grayscale value (0-15)

        std::array<PixelPairByte, BUFFER_PIXEL_WIDTH * BUFFER_PIXEL_HEIGHT / 2> buffer_; ///< Buffer to hold pixel data (4 bits per pixel)

    public:

        /**
         * @brief Default constructor for SDD1322Buffer.
         */
        SDD1322Buffer();

        /**
         * @brief Sets a pixel in the buffer.
         * The origin is at the top-left corner.
         * @param x The x-coordinate of the pixel (horizontal position).
         * @param y The y-coordinate of the pixel (vertical position).
         * @param grayscale The grayscale value to set for the pixel (0-255).
         */
        void setPixel(size_t x, size_t y, Pixel grayscale) override;

        /**
         * @brief Gets the raw data of the buffer.
         * @return A pointer to the raw data of the buffer.
         * @note The data is stored in a packed format, with 2 pixels per byte (4 bits per pixel).
         */
        [[nodiscard]]
        constexpr const uint8_t* data() const override;

        /**
         * @brief Gets the size of the buffer in bytes.
         * @return The size of the buffer in bytes.
         * @note The size is calculated as BUFFER_WIDTH * BUFFER_HEIGHT / 2,
         * since each byte contains 2 pixels (4 bits per pixel).
         */
        [[nodiscard]]
        constexpr const size_t size() const override;

        /**
         * @brief Gets the width of the buffer.
         * @return The width of the buffer in pixels.
         */
        constexpr size_t getWidth() const override;

        /**
         * @brief Gets the height of the buffer.
         * @return The height of the buffer in pixels.
         */
        constexpr size_t getHeight() const override;

    private:

        /**
         * @brief Converts an 8-bit grayscale pixel to a 4-bit grayscale pixel.
         * @param _8BitGrayScale The 8-bit grayscale pixel value (0-255).
         * @return The corresponding 4-bit grayscale pixel value (0-15).
         */
        inline static PixelGrayscale get4BitGrayscale(Pixel _8BitGrayScale);

    };

    // Inline method implementations

    constexpr const uint8_t* SDD1322Buffer::data() const {
        return buffer_.data();
    }

    constexpr const size_t SDD1322Buffer::size() const {
        return buffer_.size();
    }

    constexpr size_t SDD1322Buffer::getWidth() const {
        return BUFFER_PIXEL_WIDTH;
    }

    constexpr size_t SDD1322Buffer::getHeight() const {
        return BUFFER_PIXEL_HEIGHT;
    }

    inline SDD1322Buffer::PixelGrayscale SDD1322Buffer::get4BitGrayscale(Pixel _8BitGrayScale) {
        return _8BitGrayScale / 16; // Convert 8-bit grayscale to 4-bit grayscale (0-15)
    }

} // namespace PiAlarm::gfx

#endif //SDD1322BUFFER_H
