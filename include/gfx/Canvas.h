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
            IgnoreBlack,        ///< Ignore black pixels when drawing
            DisplayAll,         ///< Display all pixels without any filtering
            Invert,             ///< Invert the pixel colors when drawing. Pixels that are 0 (black) after inversion will not be drawn.
            InvertAndDisplayAll ///< Inverted drawing mode that displays all pixels
        };

        /**
         * @enum Anchor
         * @brief Specifies the anchor point used to position text on the canvas.
         *
         * This enum defines which point of the text bounding box is aligned with the
         * given (x, y) coordinates in text rendering functions. It provides precise
         * control over text placement on the screen.
         */
        enum class Anchor : uint8_t {
            TopLeft,       ///< Aligns the top-left corner of the text box with (x, y).
            MiddleLeft,    ///< Aligns the vertical center of the left edge with (x, y).
            BottomLeft,    ///< Aligns the bottom-left corner of the text box with (x, y).
            TopCenter,     ///< Aligns the top-center of the text box with (x, y).
            Center,        ///< Aligns the center of the text box with (x, y).
            BottomCenter,  ///< Aligns the bottom-center of the text box with (x, y).
            TopRight,      ///< Aligns the top-right corner of the text box with (x, y).
            MiddleRight,   ///< Aligns the vertical center of the right edge with (x, y).
            BottomRight    ///< Aligns the bottom-right corner of the text box with (x, y).
        };

        /**
         * @struct DrawMetrics
         * @brief Contains metrics for the drawn content on the canvas.
         *
         * This structure holds the width and height of the drawn content in pixels.
         */
        struct DrawMetrics {
            size_t width; ///< Width of the drawn content in pixels
            size_t height; ///< Height of the drawn content in pixels
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

        /**
         * @brief Draws UTF-8 text on the canvas with the specified anchor alignment.
         *
         * Renders a  string on the canvas using the provided font,
         * with the given (x, y) coordinate interpreted according to the specified anchor.
         * The anchor defines how the text is aligned relative to (x, y), allowing precise
         * positioning in various layouts (e.g., top-left, center, bottom-right, etc.).
         *
         * @param x The horizontal position, interpreted based on the anchor.
         * @param y The vertical position, interpreted based on the anchor.
         * @param text The text to draw.
         * @param font The font used to render the text.
         * @param anchor The anchor point that determines how the text is aligned
         *               relative to (x, y). Defaults to Anchor::TopLeft.
         *
         * @return DrawMetrics containing the width and height of the rendered text.
         *
         * @see Anchor for available alignment options.
         */
        DrawMetrics drawText(size_t x, size_t y, const std::string& text, const std::shared_ptr<IFont>& font, Anchor anchor = Anchor::TopLeft) const;

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
        std::vector<PositionedGlyph> layoutText(const std::string& text, const std::shared_ptr<IFont>& font) const;

        /**
         * @brief Measures the width and height of the laid-out text.
         * This method calculates the total width and height of the text based on the rendered glyphs.
         * @param glyphs The vector of PositionedGlyphs representing the laid-out text.
         * @param font The Font used to render the text.
         * @return A pair containing the width and height of the text in pixels.
         */
        std::pair<size_t, size_t> measureText(const std::vector<PositionedGlyph>& glyphs, const std::shared_ptr<IFont>& font) const;

        /**
         * @brief Gets the maximum ascender value from the laid-out glyphs.
         * This method finds the maximum ascender value (bearing Y) of the glyphs,
         * which is used to adjust the vertical position of the text.
         * @param glyphs The vector of PositionedGlyphs representing the laid-out text.
         * @return The maximum ascender value among the glyphs, as a positive integer.
         */
        int getMaxAscender(const std::vector<PositionedGlyph>& glyphs) const;

        /**
         * @brief Gets the maximum descender value from the laid-out glyphs.
         * This method finds the maximum descender value of the glyphs,
         * calculated as a negative integer representing the distance below the baseline.
         * It corresponds to the minimal (most negative) value of (bearingY - bitmap height).
         * This value is used to adjust the vertical position of the text.
         * @param glyphs The vector of PositionedGlyphs representing the laid-out text.
         * @return The maximum descender value among the glyphs, as a negative integer.
         * @warning In this context, "maximum descender" means the glyph that extends the furthest below the baseline,
         *          so it is the smallest (most negative) value.
         *          For example, between -1 and -2, the maximum descender is -2.
         */
        int getMaxDescender(const std::vector<PositionedGlyph>& glyphs) const;

        /**
         * @brief Calculates the anchor position for text based on the specified anchor type.
         * This method determines the starting position for drawing text based on the anchor point.
         * @param x The x-coordinate where the text will be drawn.
         * @param y The y-coordinate where the text will be drawn.
         * @param textWidth The width of the text to be drawn.
         * @param maxBearingY The maximum bearing Y value of the glyphs in the text.
         *                   This is used to adjust the vertical position of the text.
         * @param font The Font used to render the text.
         * @param anchor The Anchor type that specifies how to align the text.
         * @return A pair containing the adjusted x and y coordinates for drawing the text.
         */
        std::pair<size_t, size_t> getTextAnchorPosition(
            size_t x, size_t y,
            size_t textWidth,
            int maxBearingY,
            const std::shared_ptr<IFont>& font,
            Anchor anchor
        ) const;

    public:

        /**
         * @brief Gets the buffer used for drawing.
         * @return A constant reference to the IBuffer instance.
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
