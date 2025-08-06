#include <utility>

#include "gfx/Canvas.h"

namespace PiAlarm::gfx {

    Canvas::Canvas(std::unique_ptr<IBuffer> buffer)
        : buffer_{std::move(buffer)}
    {}

} // namespace PiAlarm::gfx
