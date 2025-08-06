#ifndef TYPES_H
#define TYPES_H

#include <cstdint> // For uint8_t


/**
 * @namespace PiAlarm::gfx
 * @brief Contains graphics-related classes and interfaces for the PiAlarm application.
 *
 * This namespace contains types and interfaces used for graphics rendering,
 * including pixel representation, font rendering, and buffer management.
 * It provides a foundation for drawing graphics on various display devices.
 */
namespace PiAlarm::gfx {

    using Pixel = uint8_t; ///< Type alias for a pixel, typically 8 bits (1 byte) representing grayscale (0 to 255).

} // namespace PiAlarm::gfx

#endif //TYPES_H
