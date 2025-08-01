#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "AudioTypes.h"
#include "BassContext.hpp"
#include "logging/HasLogger.h"
#include <filesystem>
#include <optional>
#include <utility>
#include <vector>
#include <thread>
#include <atomic>

namespace PiAlarm::media {

    /**
     * @class MusicPlayer
     * @brief A class to manage and play music tracks from a specified folder.
     *
     * Audio files are played in a loop or in a shuffled playlist mode with smooth crossfade transitions.
     * This class utilizes the BASS audio library to play and manage audio streams.
     */
    class MusicPlayer : public logging::HasLogger {
        BassContext bassContext_; ///< RAII context for BASS initialization and cleanup.

        std::atomic<bool> running_; ///< Flag to indicate if the player is running.
        std::thread playerThread_; ///< Thread running the music playback loop.

        static constexpr float FADE_DURATION = 3.0f; ///< Duration for volume fade in seconds.

    public:
        using Track = std::filesystem::path; ///< Type alias for a single audio track path.
        using Playlist = std::vector<Track>; ///< Type alias for a playlist of audio file paths.

        /**
         * @brief Constructor for MusicPlayer.
         * Initializes the BASS audio library and prepares the player for playback.
         */
        MusicPlayer();

        /**
         * @brief Destructor for MusicPlayer.
         * Stops playback and releases all associated resources.
         */
        ~MusicPlayer() override;

        /**
         * @brief Starts the music playback loop with the specified playlist.
         * If the playlist contains only one track, it plays that track in a loop.
         * If multiple tracks are available, it plays them with crossfade transitions.
         * @param playlist A vector of strings containing paths to audio files.
         */
        void start(const Playlist& playlist);

        /**
         * @brief Stops the music playback loop and releases audio resources.
         */
        void stop();

        /**
         * @brief Checks if the given path is a playable audio file.
         * @note This method loads the file to verify its playability.
         * @param path Path to the audio file.
         * @return True if the file is playable, false otherwise.
         */
        static bool isPlayable(const std::filesystem::path& path);

    private:

        /**
         * @brief The main loop for the music player.
         * This function runs in a separate thread and handles playback of the playlist.
         * It plays tracks in a loop or with crossfade transitions as needed.
         * @param playlist The playlist to play, represented as a vector of Track paths.
         */
        void playerLoop(Playlist playlist);

        /**
         * @brief Plays a single track in a loop.
         * This method is used when the playlist contains only one track.
         * It plays the track continuously until stopped.
         * @param track The track to play, represented as a Track path.
         */
        void playSingleTrackLooped(const Track& track);

        /**
         * @brief Plays a playlist with crossfade transitions between tracks.
         * This method handles the playback of multiple tracks with smooth volume transitions.
         * @param playlist The playlist to play, represented as a vector of Track paths.
         */
        void playPlaylistWithCrossfade(const Playlist& playlist);

        /**
         * @brief Starts playback of the track at the given index.
         * @param track The track to play, represented as a string path.
         * @return AudioStream handle representing the active stream or an invalid handle if playback failed.
         * @note This method does not check if the track is playable.
         * @note Always verify the stream is valid before using it.
         */
        AudioStream playTrack(const Track& track);

        /**
         * @brief Finds the next playable track in the playlist starting from a given index.
         * This method attempts to play tracks until it finds one that is playable.
         * If no playable track is found, the optional is empty
         * @param playlist The playlist to search through.
         * @param startIndex The index to start searching from.
         * @return A pair containing the index and AudioStream of the playable track, or an empty optional if none found.
         */
        std::optional<std::pair<size_t, AudioStream>> findNextPlayableTrack(
            const Playlist& playlist, size_t startIndex
        );

        /**
         * @brief Waits until the current track is near its end.
         * Used to time the transition between tracks before the last few seconds.
         * @param stream The current audio stream being played.
         */
        void waitBeforeTransition(AudioStream stream);

        /**
         * @brief Smoothly fades the volume of an audio channel.
         * Interpolates volume linearly from start to end over the given duration.
         * @param channel The audio channel to fade.
         * @param start The starting volume level (0.0 to 1.0).
         * @param end The ending volume level (0.0 to 1.0).
         * @param duration The duration of the fade in seconds.
         */
        void fadeVolume(AudioChannel channel, float start, float end, float duration);

        /**
         * @brief Fades in the audio channel to full volume.
         * This is typically used when starting playback of a new track.
         * @param channel The audio channel to fade in.
         */
        void fadeIn(AudioChannel channel);

        /**
         * @brief Fades out the audio channel to silence.
         * This is typically used when stopping playback of a track.
         * @param channel The audio channel to fade out.
         */
        void fadeOut(AudioChannel channel);

        /**
         * @brief Performs a crossfade between two audio streams.
         * Gradually lowers the volume of the current stream while increasing the volume of the next.
         * @param current The current audio stream.
         * @param next The next audio stream to play.
         */
        void crossfade(AudioStream current, AudioStream next);

        /**
         * @brief Stops and frees the specified audio stream.
         * @param stream The audio stream to clean up.
         */
        void cleanupStream(AudioStream stream);

    };

} // namespace PiAlarm::media

#endif // MUSICPLAYER_H
