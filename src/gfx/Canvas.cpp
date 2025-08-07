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

    void Canvas::drawChar(size_t x, size_t y, const UTF8Char& utf8Char, IFont& font) const {
        const auto glyph = font.renderChar(utf8Char);

        const size_t baselineY = y + font.getAscender();

        drawGlyph(x, baselineY, glyph);
    }

    void Canvas::drawGlyph(size_t x, size_t baselineY, const RenderedGlyph &glyph) const {
        const size_t drawX = x + glyph.bearingX;
        const size_t drawY = baselineY - glyph.bearingY;

        drawBitmap(drawX, drawY, glyph.bitmap);
    }

    Canvas::DrawMetrics Canvas::drawText(size_t x, size_t y, const std::string& text, IFont& font) const {
        auto glyphs = layoutText(text, font);
        size_t baselineY = y + font.getAscender();

        for (const auto& g : glyphs) {
            drawGlyph(x + g.xOffset, baselineY, g.glyph);
        }

        auto [textWidth, textHeight] = measureText(glyphs, font);
        return {textWidth, textHeight};
    }

    Canvas::DrawMetrics Canvas::drawTextCentered(size_t centerX, size_t centerY, const std::string& text, IFont& font) const {
        auto glyphs = layoutText(text, font);

        auto [textWidth, textHeight] = measureText(glyphs, font);
        size_t startX = centerX - textWidth / 2;
        size_t baselineY = centerY - textHeight / 2 + font.getAscender();

        for (const auto& g : glyphs) {
            drawGlyph(startX + g.xOffset, baselineY, g.glyph);
        }

        return {textWidth, textHeight};
    }

    std::vector<PositionedGlyph> Canvas::layoutText(const std::string& text, IFont& font) const {
        std::vector<PositionedGlyph> glyphs;
        size_t cursorX = 0;

        auto it = text.begin();
        auto end = text.end();

        while (it != end) {
            UnicodeChar codepoint = utf8::next(it, end);
            RenderedGlyph glyph = font.renderChar(codepoint);
            glyphs.emplace_back(std::move(glyph), cursorX);
            cursorX += glyphs.back().glyph.advance;
        }

        return glyphs;
    }

    std::pair<size_t, size_t> Canvas::measureText(const std::vector<PositionedGlyph>& glyphs, const IFont& font) const {
        if (glyphs.empty()) return {0, 0};

        size_t width = glyphs.back().xOffset + glyphs.back().glyph.advance; // offset of the last glyph + its advance
        size_t height = font.getLineHeight();
        return {width, height};
    }

} // namespace PiAlarm::gfx
