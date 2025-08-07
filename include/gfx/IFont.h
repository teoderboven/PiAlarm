#ifndef FONT_H
#define FONT_H

#include "gfx/Glyph.h"

namespace PiAlarm::gfx {

    /**
     * @interface IFont
     * @brief Represents a font interface for rendering characters.
     *
     * This interface defines methods for rendering characters, getting font metrics,
     * and managing the bitmap representation of rendered glyphs.
     */
    class IFont {
    public:
        using UnicodeChar = uint32_t; ///< Alias for Unicode character representation
        using UTF8Char = std::string; ///< Alias for UTF-8 encoded character representation

        /**
         * @brief Default destructor for Font.
         * Ensures proper cleanup of derived classes.
         */
        virtual ~IFont() = default;

        /**
         * @brief Renders a character and returns its bitmap representation.
         * @param utf8Char The character to render, represented as a UTF-8 encoded string.
         * @return A GlyphBitmap containing the rendered character's bitmap data.
         */
        virtual const RenderedGlyph renderChar(const UTF8Char& utf8Char) = 0;

        /**
         * @brief Renders a character and returns its bitmap representation.
         * @param codepoint The Unicode codepoint of the character to render.
         * @return A RenderedGlyph containing the rendered character's bitmap data.
         */
        virtual const RenderedGlyph renderChar(UnicodeChar codepoint) = 0;

        /**
         * @brief Gets the ascender of the font.
         * The ascender is the distance from the baseline to the highest point of the font.
         * @return The ascender value in pixels.
         */
        [[nodiscard]]
        virtual int getAscender() const = 0;

        /**
         * @brief Gets the descender of the font.
         * The descender is the distance from the baseline to the lowest point of the font.
         * @return The descender value in pixels.
         */
        [[nodiscard]]
        virtual int getDescender() const = 0;

        /**
         * @brief Gets the line height of the font.
         * The line height is the total height of a line of text, including ascender and descender.
         * @return The line height value in pixels.
         */
        [[nodiscard]]
        virtual int getLineHeight() const = 0;

    };

} // namespace PiAlarm::gfx

#endif //FONT_H
