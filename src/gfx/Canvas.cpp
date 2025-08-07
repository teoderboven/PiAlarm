#include <utility>
#include "utf8.h"

#include "gfx/Canvas.h"

namespace PiAlarm::gfx {

    Canvas::Canvas(std::unique_ptr<IBuffer> buffer, DrawMode drawMode)
        : buffer_{std::move(buffer)}, drawMode_{drawMode}
    {}

    void Canvas::setPixel(size_t x, size_t y, Pixel value) const {
        uint8_t finalValue {value};

        switch (drawMode_) {
            case DrawMode::DisplayAll:
                break;
            case DrawMode::IgnoreBlack:
                if (value == 0) return;
                break;
            case DrawMode::Invert:
                if (value == 255) return; // will become black
            case DrawMode::InvertAndDisplayAll:
                finalValue = 255 - value;
                break;
        }

        buffer_->setPixel(x, y, finalValue);
    }

    void Canvas::drawBitmap(size_t x, size_t y, const Bitmap &bitmap) const {
        for (size_t row = 0; row < bitmap.height; row++) {
            for (size_t col = 0; col < bitmap.width; col++) {
                setPixel(x + col, y + row, bitmap.getPixel(col, row));
            }
        }
    }

    void Canvas::drawChar(size_t x, size_t y, const IFont::UTF8Char& utf8Char, IFont& font) const {
        const auto glyph = font.renderChar(utf8Char);

        const size_t baselineY = y + font.getAscender();

        drawGlyph(x, baselineY, glyph);
    }

    void Canvas::drawGlyph(size_t x, size_t baselineY, const RenderedGlyph &glyph) const {
        const size_t drawX = x + glyph.bearingX;
        const size_t drawY = baselineY - glyph.bearingY;

        drawBitmap(drawX, drawY, glyph.bitmap);
    }

    void Canvas::drawText(size_t x, size_t y, const std::string &text, IFont &font) const {
        size_t cursorX = x;
        const size_t baselineY = y + font.getAscender();

        auto it = text.begin();
        const auto end = text.end();

        while (it != end) {
            IFont::UnicodeChar codepoint = utf8::next(it, end); // Decode the next UTF-8 character
            auto glyph = font.renderChar(codepoint);

            drawGlyph(cursorX, baselineY, glyph);

            cursorX += glyph.advance; // Move the cursor to the right by the advance of the glyph. Used to calculate the next character position
        }
    }


} // namespace PiAlarm::gfx
