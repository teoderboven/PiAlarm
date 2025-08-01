#ifndef MUSICSERVICE_H
#define MUSICSERVICE_H

#include <filesystem>

#include "media/MusicPlayer.h"
#include "logging/HasLogger.h"

namespace PiAlarm::media {

    namespace fs = std::filesystem;

    /**
     * @class MusicService
     * @brief A service to manage and play music tracks from specified folders.
     *
     * This service initializes a MusicPlayer instance to handle audio playback.
     * It loads a playlist from a primary folder and falls back to a secondary folder if necessary.
     */
    class MusicService : public logging::HasLogger {
        const fs::path folder_; ///< Path to the primary folder containing audio files.
        const fs::path fallbackFolder_; ///< Path to the fallback folder if the primary is empty or invalid.
        MusicPlayer musicPlayer_; ///< MusicPlayer instance to handle audio playback.

    public:

        /**
         * @brief Constructor for MusicService.
         * Initializes the music player with the specified folder or fallback folder.
         * @param folder Path to the primary folder containing audio files.
         * @param fallbackFolder Path to the fallback folder if the primary is empty or invalid.
         */
        MusicService(const fs::path& folder, const fs::path& fallbackFolder);

        /**
         * @brief Starts the music playback service.
         * Loads a valid playlist from the specified folder and starts the music player.
         */
        void start();

        /**
         * @brief Stops the music playback service.
         * Stops the music player and releases all associated resources.
         */
        inline void stop();

    };

    // Inline method implementation

    inline void MusicService::stop() {
        musicPlayer_.stop();
    }

} // namespace PiAlarm::media

#endif //MUSICSERVICE_H
