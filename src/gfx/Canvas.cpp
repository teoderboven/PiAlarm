#include <utility>
#include <algorithm>
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

    void Canvas::drawRectangle(size_t x, size_t y, size_t w, size_t h, size_t thickness, Pixel color) const {
        if (w <= 0 || h <= 0 || thickness <= 0) return;

        // Top
        for (size_t i {x}; i < x + w; ++i) {
            for (size_t t {0}; t < thickness; ++t) {
                setPixel(i, y + t, color);
            }
        }
        // Bottom
        for (size_t i {x}; i < x + w; ++i) {
            for (size_t t {0}; t < thickness; ++t) {
                setPixel(i, y + h - 1 - t, color);
            }
        }
        // Left
        for (size_t j {y}; j < y + h; ++j) {
            for (size_t t {0}; t < thickness; ++t) {
                setPixel(x + t, j, color);
            }
        }
        // Right
        for (size_t j {y}; j < y + h; ++j) {
            for (size_t t {0}; t < thickness; ++t) {
                setPixel(x + w - 1 - t, j, color);
            }
        }
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

    Canvas::DrawMetrics Canvas::drawText(size_t x, size_t y, const std::string& text, const std::shared_ptr<IFont>& font, Anchor anchor) const {
        const auto glyphs = layoutText(text, font);

        // get measures of the text
        auto [textWidth, textHeight] = measureText(glyphs, font);
        auto maxBearingY = getMaxAscender(glyphs);

        // Adjust the x and y coordinates based on the anchor
        auto [drawX, baselineY] = getTextAnchorPosition(x, y, textWidth, maxBearingY, font, anchor);

        // Draw the text
        for (const auto& g : glyphs) {
            drawGlyph(drawX + g.xOffset, baselineY, g.glyph);
        }

        return {textWidth, textHeight};
    }

    std::vector<PositionedGlyph> Canvas::layoutText(const std::string& text, const std::shared_ptr<IFont>& font) const {
        std::vector<PositionedGlyph> glyphs;
        size_t cursorX = 0;

        auto it = text.begin();
        auto end = text.end();

        while (it != end) {
            UnicodeChar codepoint = utf8::next(it, end);
            RenderedGlyph glyph = font->renderChar(codepoint);
            glyphs.emplace_back(std::move(glyph), cursorX);
            cursorX += glyphs.back().glyph.advance;
        }

        return glyphs;
    }

    std::pair<size_t, size_t> Canvas::measureText(const std::vector<PositionedGlyph>& glyphs, const std::shared_ptr<IFont>& font) const {
        if (glyphs.empty()) return {0, 0};

        size_t width = glyphs.back().xOffset + glyphs.back().glyph.advance; // offset of the last glyph + its advance
        size_t height = getMaxAscender(glyphs) - getMaxDescender(glyphs);
        return {width, height};
    }

    int Canvas::getMaxAscender(const std::vector<PositionedGlyph>& glyphs) const {
        if (glyphs.empty()) return 0;

        return std::ranges::max_element(
            glyphs,
            [](const PositionedGlyph& a, const PositionedGlyph& b) {
                return a.glyph.bearingY < b.glyph.bearingY;
            })->glyph.bearingY;
    }

    int Canvas::getMaxDescender(const std::vector<PositionedGlyph>& glyphs) const {
        if (glyphs.empty()) return 0;

        auto glyphWithMax = std::ranges::min_element(
            glyphs,
            [](const PositionedGlyph& a, const PositionedGlyph& b) {
                return (a.glyph.bearingY - a.glyph.bitmap.height) < (b.glyph.bearingY - b.glyph.bitmap.height);
            });

        return glyphWithMax->glyph.bearingY - glyphWithMax->glyph.bitmap.height; // return descender as negative value
    }

    std::pair<size_t, size_t> Canvas::getTextAnchorPosition(
        size_t x, size_t y,
        size_t textWidth,
        int maxBearingY,
        const std::shared_ptr<IFont>& font,
        Anchor anchor) const
    {
        size_t drawX = x;
        size_t baselineY = y;
        auto ascender = font->getAscender();
        auto descender = font->getDescender();

        // horizontal offset
        switch (anchor) {
            case Anchor::TopCenter:
            case Anchor::Center:
            case Anchor::BottomCenter:
                drawX -= textWidth / 2;
                break;

            case Anchor::TopRight:
            case Anchor::MiddleRight:
            case Anchor::BottomRight:
                drawX -= textWidth;
                break;

            default:
                break; // left = unchanged x
        }

        // vertical offset
        switch (anchor) {
            case Anchor::TopLeft:
            case Anchor::TopCenter:
            case Anchor::TopRight:
                baselineY += maxBearingY;
                break;

            case Anchor::MiddleLeft:
            case Anchor::Center:
            case Anchor::MiddleRight:
                baselineY += (ascender + descender) / 2; // descender is negative
                break;

            default:
                break; // bottom = unchanged y
        }

        return {drawX, baselineY};
    }

} // namespace PiAlarm::gfx
