#ifndef CANVAS_H
#define CANVAS_H

#include <memory>

#include "gfx/IBuffer.h"

namespace PiAlarm::gfx {

    /**
     * @class Canvas
     * @brief Represents a drawable canvas using a graphics buffer.
     *
     * This class provides methods to draw on a canvas by manipulating pixels, clear the canvas, and access the underlying buffer.
     * It is designed to work with a unique pointer to an IBuffer instance, which handles the pixel data.
     */
    class Canvas {
        std::unique_ptr<IBuffer> buffer_; ///< Unique pointer to the buffer used for drawing

    public:

        using Pixel = IBuffer::Pixel; ///< Type alias for a pixel, typically 8 bits (1 byte) representing grayscale (0 to 255)

        /**
         * Constructor for Canvas.
         * Initializes the canvas with a unique pointer to an IBuffer.
         * @param buffer Unique pointer to the buffer used for drawing.
         */
        explicit Canvas(std::unique_ptr<IBuffer> buffer);

        /**
         * Clears the canvas by resetting the buffer.
         * This method should be called before drawing new content.
         */
        inline void clear() const;

        /**
         * Sets a pixel in the canvas at the specified coordinates.
         * @param x The x-coordinate of the pixel (horizontal position).
         * @param y The y-coordinate of the pixel (vertical position).
         * @param grayscale The grayscale value to set for the pixel (0-255).
         */
        inline void setPixel(size_t x, size_t y, Pixel grayscale) const;

        /**
         * Gets the buffer used for drawing.
         * @return A constant reference to the IBuffer instance.
         * @note The returned buffed cannot be stored in a variable as the return type is a reference to the Interface.
         */
        [[nodiscard]]
        inline const IBuffer& buffer() const;

        /**
         * Gets the width of the buffer in pixels.
         * @return The width of the buffer in pixels.
         */
        [[nodiscard]]
        constexpr size_t getWidth() const;

        /**
         * Gets the height of the buffer in pixels.
         * @return The height of the buffer in pixels.
         */
        [[nodiscard]]
        constexpr size_t getHeight() const;

    };

    // Inline methods implementation

    inline void Canvas::clear() const {
        buffer_->clear();
    }

    inline void Canvas::setPixel(size_t x, size_t y, Pixel grayscale) const {
        buffer_->setPixel(x, y, grayscale);
    }

    inline const IBuffer& Canvas::buffer() const {
        return *buffer_;
    }

    constexpr size_t Canvas::getWidth() const {
        return buffer_->getWidth();
    }

    constexpr size_t Canvas::getHeight() const {
        return buffer_->getHeight();
    }

} // namespace PiAlarm::gfx

#endif //CANVAS_H
