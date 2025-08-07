#ifndef TRUETYPEFONT_H
#define TRUETYPEFONT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <mutex>

#include "gfx/IFont.h"

namespace PiAlarm::gfx {

    /**
     * @class TrueTypeFont
     * @brief Represents a TrueType font for rendering characters.
     *
     * This class uses FreeType to load and render TrueType fonts, providing methods
     * to render characters and retrieve font metrics such as ascender, descender, and line height.
     */
    class TrueTypeFont : public IFont {
        static FT_Library ftLibrary; ///< FreeType library instance, initialized once
        static std::once_flag ftInitFlag; ///< Flag to ensure FreeType is initialized only once

        FT_Face face; ///< FreeType face object representing the loaded font

    public:

        /**
         * @brief Constructs a TrueTypeFont object.
         * Initializes the FreeType library and loads the font from the specified path.
         * @param fontPath The path to the TrueType font file.
         * @param pixelHeight The height of the font in pixels.
         * @throws std::runtime_error if the font cannot be loaded or initialized.
         */
        TrueTypeFont(const std::string& fontPath, int pixelHeight);

        /**
         * @brief Default destructor for TrueTypeFont.
         * Cleans up the FreeType face object.
         */
        ~TrueTypeFont() override;

        // Inherited from IFont
        /**
         * @brief Renders a character and returns its bitmap representation.
         * @param utf8Char The character to render, represented as a UTF-8 encoded string.
         * @return A RenderedGlyph containing the rendered character's bitmap data.
         * @throws std::runtime_error if the character cannot be loaded or rendered.
         */
        const RenderedGlyph renderChar(const UTF8Char& utf8Char) override;

        /**
         * @brief Renders a character and returns its bitmap representation.
         * @param codepoint The Unicode codepoint of the character to render.
         * @return A RenderedGlyph containing the rendered character's bitmap data.
         * @throws std::runtime_error if the character cannot be loaded or rendered.
         */
        const RenderedGlyph renderChar(UnicodeChar codepoint) override;

        [[nodiscard]]
        inline int getAscender() const override;
        [[nodiscard]]
        inline int getDescender() const override;
        [[nodiscard]]
        inline int getLineHeight() const override;

    };

    // inline methods implementation

    inline int TrueTypeFont::getAscender() const {
        if (face) {
            return face->size->metrics.ascender >> 6; // Convert from 26.6 fixed-point to pixels
                                                      // What is 26.6 fixed-point : https://lists.gnu.org/archive/html/freetype/2002-09/msg00076.html
        }
        return 0;
    }

    inline int TrueTypeFont::getDescender() const {
        if (face) {
            return face->size->metrics.descender >> 6;
        }
        return 0;
    }

    inline int TrueTypeFont::getLineHeight() const {
        if (face) {
            return face->size->metrics.height >> 6;
        }
        return 0;
    }

} // namespace PiAlarm::gfx

#endif //TRUETYPEFONT_H
