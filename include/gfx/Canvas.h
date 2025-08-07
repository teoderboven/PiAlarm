#ifndef CANVAS_H
#define CANVAS_H

#include <memory>

#include "gfx/Types.h"
#include "gfx/Bitmap.h"
#include "gfx/IBuffer.h"
#include "gfx/IFont.h"

namespace PiAlarm::gfx {

    /**
     * @class Canvas
     * @brief Represents a drawable canvas using a graphics buffer.
     *
     * This class provides methods to draw on a canvas by manipulating pixels, clear the canvas, and access the underlying buffer.
     * It is designed to work with a unique pointer to an IBuffer instance, which handles the pixel data.
     */
    class Canvas {
    public:
        /**
         * @enum DrawMode
         * @brief Specifies the drawing mode for the canvas.
         *
         * This enum defines different modes for drawing on the canvas, such as normal drawing,
         * ignoring black pixels, inverting colors, or both ignoring black pixels and inverting colors.
         */
        enum class DrawMode : uint8_t {
            IgnoreBlack, ///< Ignore black pixels when drawing
            DisplayAll, ///< Display all pixels without any filtering
            Invert, ///< Invert the pixel colors when drawing. Pixels that are 0 (black) after inversion will not be drawn.
            InvertAndDisplayAll ///< Inverted drawing mode that displays all pixels
        };

    private:
        std::unique_ptr<IBuffer> buffer_; ///< Unique pointer to the buffer used for drawing
        DrawMode drawMode_; ///< Current drawing mode for the canvas

    public:

        /**
         * @brief Constructor for Canvas.
         * Initializes the canvas with a unique pointer to an IBuffer.
         * @param buffer Unique pointer to the buffer used for drawing.
         * @param drawMode The drawing mode for the canvas, default is DrawMode::Normal.
         */
        explicit Canvas(std::unique_ptr<IBuffer> buffer, DrawMode drawMode = DrawMode::IgnoreBlack);

        /**
         * @brief Sets the drawing mode for the canvas.
         * @param drawMode The drawing mode to set.
         */
        inline void setDrawMode(DrawMode drawMode);

        /**
         * @brief Gets the current drawing mode of the canvas.
         * @return The current drawing mode.
         */
        [[nodiscard]]
        inline DrawMode getDrawMode() const;

        /**
         * @brief Clears the canvas by resetting the buffer.
         * This method should be called before drawing new content.
         */
        inline void clear() const;

        /**
         * @brief Sets a pixel in the canvas at the specified coordinates.
         * @param x The x-coordinate of the pixel (horizontal position).
         * @param y The y-coordinate of the pixel (vertical position).
         * @param grayscale The grayscale value to set for the pixel (0-255).
         */
        inline void drawPixel(size_t x, size_t y, Pixel grayscale) const;

        /**
         * @brief Draws a bitmap at the specified coordinates on the canvas.
         * @param x The x-coordinate where the bitmap will be drawn.
         * @param y The y-coordinate where the bitmap will be drawn.
         * @param bitmap The Bitmap object to draw on the canvas.
         */
        void drawBitmap(size_t x, size_t y, const Bitmap& bitmap) const;

        /**
         * @brief Draws a character at the specified coordinates using the provided font.
         * @param x The x-coordinate where the character will be drawn.
         * @param y The y-coordinate where the character will be drawn.
         * @param utf8Char The character to draw, represented as a UTF-8 encoded string.
         * @param font The Font used to render the character.
         */
        void drawChar(size_t x, size_t y, const UTF8Char& utf8Char, IFont& font) const;

    private:

        /**
         * @brief Draws a glyph at the specified coordinates.
         * This method is used internally to draw a rendered glyph on the canvas.
         * @param x The x-coordinate where the glyph will be drawn.
         * @param baselineY The y-coordinate of the baseline for the glyph.
         * @param glyph The RenderedGlyph object containing the glyph data to draw.
         */
        void drawGlyph(size_t x, size_t baselineY, const RenderedGlyph& glyph) const;

        /**
         * @brief Lays out the text by rendering each character and positioning them.
         * This method processes the input text and returns a vector of PositionedGlyphs,
         * which contain the rendered glyphs and their respective x-offsets.
         * @param text The text to layout, represented as a standard string.
         * @param font The Font used to render the text.
         * @return A vector of PositionedGlyphs representing the laid-out text.
         */
        std::vector<PositionedGlyph> layoutText(const std::string& text, IFont& font) const;

        /**
         * @brief Measures the width and height of the laid-out text.
         * This method calculates the total width and height of the text based on the rendered glyphs.
         * @param glyphs The vector of PositionedGlyphs representing the laid-out text.
         * @param font The Font used to render the text.
         * @return A pair containing the width and height of the text in pixels.
         */
        std::pair<size_t, size_t> measureText(const std::vector<PositionedGlyph>& glyphs, const IFont& font) const;

    public:

        /**
         * @brief Draws text at the specified coordinates using the provided font.
         * @param x The x-coordinate where the text will be drawn.
         * @param y The y-coordinate where the text will be drawn.
         * @param text The text to draw, represented as a standard string.
         * @param font The Font used to render the text.
         */
        void drawText(size_t x, size_t y, const std::string& text, IFont& font) const;

        /**
         * @brief Draws text centered at the specified coordinates using the provided font.
         * This method calculates the starting position to center the text based on its width and height.
         * @param centerX The x-coordinate of the center point where the text will be drawn.
         * @param centerY The y-coordinate of the center point where the text will be drawn.
         * @param text The text to draw, represented as a standard string.
         * @param font The Font used to render the text.
         */
        void drawTextCentered(size_t centerX, size_t centerY, const std::string& text, IFont& font) const;

        /**
         * @brief Gets the buffer used for drawing.
         * @return A constant reference to the IBuffer instance.
         * @note The returned buffed cannot be stored in a variable as the return type is a reference to the Interface.
         */
        [[nodiscard]]
        inline const IBuffer& buffer() const;

        /**
         * @brief Gets the width of the buffer in pixels.
         * @return The width of the buffer in pixels.
         */
        [[nodiscard]]
        constexpr size_t getWidth() const;

        /**
         * @brief Gets the height of the buffer in pixels.
         * @return The height of the buffer in pixels.
         */
        [[nodiscard]]
        constexpr size_t getHeight() const;

    private:

        /**
         * @brief Sets a pixel in the buffer at the specified coordinates.
         * This method applies the current drawing mode before setting the pixel value.
         * @param x The x-coordinate of the pixel (horizontal position).
         * @param y The y-coordinate of the pixel (vertical position).
         * @param value The pixel value to set (0-255).
         */
        void setPixel(size_t x, size_t y, Pixel value) const;

    };

    // Inline methods implementation

    inline void Canvas::setDrawMode(DrawMode drawMode) {
        drawMode_ = drawMode;
    }

    inline Canvas::DrawMode Canvas::getDrawMode() const {
        return drawMode_;
    }

    inline void Canvas::clear() const {
        buffer_->clear();
    }

    inline void Canvas::drawPixel(size_t x, size_t y, Pixel grayscale) const {
        setPixel(x, y, grayscale);
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
