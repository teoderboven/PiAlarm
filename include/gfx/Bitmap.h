#ifndef BITMAP_H
#define BITMAP_H

#include <vector>
#include <algorithm>
#include "gfx/Types.h"

namespace PiAlarm::gfx {

    /**
     * @struct Bitmap
     * @brief Represents a bitmap image with pixel data.
     *
     * This structure holds the width, height, and pixel data of a bitmap image.
     * The pixel data is stored as a flat vector of uint8_t values, where each value represents
     * the grayscale intensity of a pixel (0-255).
     */
    struct Bitmap {
        size_t width; ///< The width of the bitmap in pixels
        size_t height; ///< The height of the bitmap in pixels
        std::vector<Pixel> pixels; ///< The pixel data of the bitmap, stored as a flat vector

        /**
         * @brief Constructs a Bitmap with the specified width and height.
         * Initializes the pixel data to zero (black).
         * @param width The width of the bitmap in pixels
         * @param height The height of the bitmap in pixels
         */
        explicit Bitmap(size_t width = 0, size_t height = 0);

        /**
         * @brief Gets the pixel value at the specified coordinates.
         * @param x The x-coordinate of the pixel (horizontal position)
         * @param y The y-coordinate of the pixel (vertical position)
         * @return The pixel value at the specified coordinates
         */
        inline Pixel getPixel(size_t x, size_t y) const;

        /**
         * @brief Sets the pixel value at the specified coordinates.
         * @param x The x-coordinate of the pixel (horizontal position)
         * @param y The y-coordinate of the pixel (vertical position)
         * @param value The pixel value to set (0-255)
         */
        inline void setPixel(size_t x, size_t y, Pixel value);

        /**
         * @brief Fills the entire bitmap with the specified pixel value.
         * @param value The pixel value to fill the bitmap with (0-255)
         */
        inline void fill(Pixel value);

    };

    inline Bitmap::Bitmap(size_t width, size_t height)
        : width{width}, height{height}, pixels(width * height, 0)  // Initialize all pixels to 0 (black)
    {}

    inline Pixel Bitmap::getPixel(size_t x, size_t y) const {
        return pixels[y * width + x];
    }

    inline void Bitmap::setPixel(size_t x, size_t y, Pixel value) {
        pixels[y * width + x] = value;
    }

    inline void Bitmap::fill(Pixel value) {
        std::ranges::fill(pixels, value);
    }

} // namespace PiAlarm::gfx

#endif //BITMAP_H
