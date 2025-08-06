#ifndef IBUFFER_H
#define IBUFFER_H

#include <cstdint> // For uint8_t
#include <cstddef> // For size_t

/**
 * @namespace PiAlarm::gfx
 * @brief Contains graphics-related classes and interfaces for the PiAlarm application.
 *
 * This namespace includes interfaces and classes for managing graphics buffers,
 * rendering, and other graphical operations.
 */
namespace PiAlarm::gfx {

    /**
     * @interface IBuffer
     * @brief Represents a buffer for graphics data.
     *
     * This interface defines the basic operations for a graphics buffer,
     * including setting pixel values. It is intended to be implemented by
     * specific buffer classes that handle different types of graphics data.
     */
    class IBuffer {
    public:

        using Pixel = uint8_t; ///< Type alias for a pixel, typically 8 bits (1 byte) representing grayscale (0 to 255).

        /**
         * @brief Virtual destructor for IBuffer.
         * Ensures proper cleanup of derived classes.
         */
        virtual ~IBuffer() = default;

        /**
         * @brief Sets the pixel grayscale at the specified coordinates.
         * @param x The x-coordinate of the pixel (horizontal position).
         * @param y The y-coordinate of the pixel (vertical position).
         * @param grayscale The grayscale value to set for the pixel (0-255).
         */
        virtual void setPixel(size_t x, size_t y, Pixel grayscale) = 0;

        /**
         * @brief Clears the buffer by setting all pixels to zero (black).
         * This method should be called to reset the buffer before drawing new content.
         */
        virtual void clear() = 0;

        /**
         * @brief Gets the raw data of the buffer.
         * @return A pointer to the raw data of the buffer.
         * @note The data is typically stored in a packed format, depending on the specific implementation.
         */
        [[nodiscard]]
        virtual const uint8_t* data() const = 0;

        /**
         * @brief Gets the size of the buffer in bytes.
         * @return The size of the buffer in bytes.
         * @note This is typically the total number of pixels multiplied by the number of bytes per pixel.
         */
        [[nodiscard]]
        virtual size_t size() const = 0;

        /**
         * @brief Gets the width in pixels of the buffer.
         * @return The width of the buffer in pixels.
         */
        [[nodiscard]]
        virtual size_t getWidth() const = 0;

        /**
         * @brief Gets the height in pixels of the buffer.
         * @return The height of the buffer in pixels.
         */
        [[nodiscard]]
        virtual size_t getHeight() const = 0;

    };

} // namespace PiAlarm::gfx

#endif //IBUFFER_H
