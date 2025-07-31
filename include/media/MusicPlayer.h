#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "bass.h"
#include "AudioTypes.h"
#include "BassContext.hpp"
#include <string>
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
    class MusicPlayer {
        BassContext bassContext_; ///< RAII context for BASS initialization and cleanup.

        const std::string folderPath_; ///< Path to the folder containing audio files.
        std::vector<std::string> playlist_; ///< List of audio files in the playlist.
        std::atomic<bool> running_; ///< Flag to indicate if the player is running.
        std::thread playerThread_; ///< Thread running the music playback loop.

        static constexpr float FADE_DURATION = 3.0f; ///< Duration for volume fade in seconds.

    public:

        /**
         * @brief Constructor for MusicPlayer.
         * @param folderPath Path to the folder containing audio files.
         * Initializes the BASS library.
         */
        explicit MusicPlayer(std::string folderPath);

        /**
         * @brief Destructor for MusicPlayer.
         * Stops playback and releases all associated resources.
         */
        ~MusicPlayer();

        /**
         * @brief Starts the music playback loop in a separate thread.
         * Loads the playlist and plays tracks either in loop or with crossfade transitions.
         */
        void start();

        /**
         * @brief Stops the music playback loop and releases audio resources.
         */
        void stop();

    private:

        /**
         * @brief Main playback loop running in a background thread.
         * Chooses the appropriate playback strategy based on the playlist content.
         */
        void playerLoop();

        /**
         * @brief Plays a single track in an infinite loop.
         * Used when only one track is available in the playlist.
         */
        void playSingleTrackLooped();

        /**
         * @brief Plays the entire playlist with crossfade transitions.
         * Each track is played with a smooth transition to the next.
         */
        void playPlaylistWithCrossfade();

        /**
         * @brief Scans the folder and populates the playlist with audio files.
         */
        void loadPlaylist();

        /**
         * @brief Starts playback of the track at the given index.
         * @param index Index of the track to play in the playlist.
         * @return AudioStream handle representing the active stream.
         */
        AudioStream playTrack(size_t index);

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
        static void cleanupStream(AudioStream stream);

        /**
         * @brief Scans a folder for supported audio files (.mp3, .wav).
         * @param folder Path to the folder to scan.
         * @return List of audio file paths found in the folder.
         */
        static std::vector<std::string> getFiles(const std::string& folder);
    };

} // namespace PiAlarm::media

#endif // MUSICPLAYER_H
