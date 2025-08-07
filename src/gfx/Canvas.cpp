#include <utility>

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

} // namespace PiAlarm::gfx
