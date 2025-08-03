#ifndef AUDIOTYPES_H
#define AUDIOTYPES_H

#include "bass.h"

/**
 * @file AudioTypes.h
 * @brief Defines types used for audio streaming with the BASS library.
 *
 * This file contains type aliases for audio streams, channels, and positions
 * used throughout the media module of the PiAlarm application.
 */

/**
 * @namespace PiAlarm::media
 * @brief Contains types and functions related to media handling in PiAlarm.
 */
namespace PiAlarm::media {

    // Aliases for BASS types
    /**
     * @typedef AudioStream
     * @brief Represents an audio stream handle.
     * @note A stream is a channel, but not the other way around.
     */
    using AudioStream = HSTREAM;
    using AudioChannel = HCHANNEL; ///< Represents an audio channel handle.
    using AudioPosition = QWORD; ///< Represents an audio position in bytes.

} // namespace PiAlarm::media

#endif //AUDIOTYPES_H
