#include <stdexcept>
#include "utf8.h"

#include "gfx/TrueTypeFont.h"

namespace PiAlarm::gfx {

    FT_Library TrueTypeFont::ftLibrary = nullptr;
    std::once_flag TrueTypeFont::ftInitFlag;

    TrueTypeFont::TrueTypeFont(const std::string& fontPath, int pixelHeight) {

        std::call_once(ftInitFlag, []() {
            if (FT_Init_FreeType(&ftLibrary)) {
                throw std::runtime_error("Error initializing FreeType library");
            }
        });

        // load font from file
        if (FT_New_Face(ftLibrary, fontPath.c_str(), 0, &face)) {
            throw std::runtime_error("Unable to load font: " + fontPath);
        }

        if (FT_Set_Pixel_Sizes(face, 0, pixelHeight)) {
            throw std::runtime_error("Unable to set font size");
        }
    }

    TrueTypeFont::~TrueTypeFont() {
        if (face) {
            FT_Done_Face(face);
        }
    }

    const RenderedGlyph TrueTypeFont::renderChar(const UTF8Char& utf8Char) {
        auto it = utf8Char.begin();
        auto end = utf8Char.end();
        uint32_t codepoint = utf8::next(it, end); // Decode first char

        return renderChar(codepoint);
    }

    const RenderedGlyph TrueTypeFont::renderChar(UnicodeChar codepoint) {

        if (FT_Load_Char(face, codepoint, FT_LOAD_RENDER)) {
            throw std::runtime_error("Error when loading character. Codepoint = " + std::to_string(codepoint));
        }

        FT_GlyphSlot g = face->glyph;
        FT_Bitmap& bmp = g->bitmap;

        RenderedGlyph glyph{
            g->bitmap_left,
            g->bitmap_top,
            g->advance.x >> 6,
            {bmp.width, bmp.rows}
        };

        // copy FT_Bitmap pixels to glyph bitmap
        for (size_t y = 0; y < bmp.rows; ++y) {
            for (size_t x = 0; x < bmp.width; ++x) {
                glyph.bitmap.pixels[y * bmp.width + x] = bmp.buffer[y * bmp.pitch + x];
            }
        }

        return glyph;
    }

} // namespace PiAlarm::gfx
