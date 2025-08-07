#ifndef TYPES_H
#define TYPES_H

#include <cstdint> // For uint8_t & uint32_t
#include <string>


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
    using UnicodeChar = uint32_t; ///< Type alias for a Unicode character, typically 32 bits (4 bytes).
    using UTF8Char = std::string; ///< Type alias for a UTF-8 encoded character, represented as a string.

} // namespace PiAlarm::gfx

#endif //TYPES_H
