#ifndef PIALARM_GLYPH_H
#define PIALARM_GLYPH_H

#include <utility>
#include "gfx/Bitmap.h"

namespace PiAlarm::gfx {

    /**
     * @struct RenderedGlyph
     * @brief Represents a rendered glyph (character) with its bitmap representation.
     *
     * This structure contains the bearing (offset), advance (spacing), and bitmap data for a rendered character.
     */
    struct RenderedGlyph {
        int bearingX; ///< The horizontal offset from the baseline to the left side of the glyph
        int bearingY; ///< The vertical offset from the baseline to the top of the glyph
        ssize_t advance; ///< The horizontal advance (spacing) to the next glyph
        Bitmap bitmap; ///< The bitmap representation of the rendered glyph, containing pixel data

        /**
         * @brief Constructs a RenderedGlyph struct.
         * @param bearingX The horizontal offset from the baseline
         * @param bearingY The vertical offset from the baseline
         * @param advance The horizontal advance (spacing) to the next glyph
         * @param bitmap The bitmap representation of the rendered glyph
         */
        RenderedGlyph(int bearingX, int bearingY, ssize_t advance, Bitmap bitmap)
            : bearingX{bearingX}, bearingY{bearingY}, advance{advance}, bitmap{std::move(bitmap)}
        {}
    };

    /**
     * @struct PositionedGlyph
     * @brief Represents a glyph positioned at a specific horizontal offset.
     *
     * This structure contains the rendered glyph data and its horizontal offset relative to the start of the text line.
     */
    struct PositionedGlyph {
        RenderedGlyph glyph; ///< The rendered glyph data
        ssize_t xOffset; ///< The horizontal offset relative to the start of the text line where the glyph is positioned

        /**
         * @brief Constructs a PositionedGlyph struct.
         * @param glyph The rendered glyph data
         * @param xOffset The horizontal offset relative to the start of the text line
         */
        PositionedGlyph(RenderedGlyph glyph, ssize_t xOffset)
            : glyph{std::move(glyph)}, xOffset{xOffset}
        {}
    };

} // namespace PiAlarm::gfx

#endif //PIALARM_GLYPH_H
