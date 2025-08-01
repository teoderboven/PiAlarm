#include <chrono>
#include <random>
#include <algorithm>

#include "media/MusicPlayer.h"

namespace PiAlarm::media {

    MusicPlayer::MusicPlayer()
        : HasLogger("MusicPlayer"), running_(false)
    {}

    MusicPlayer::~MusicPlayer() {
        stop();
    }

    void MusicPlayer::start(const Playlist& playlist) {
        if (running_) return;

        running_ = true;
        playerThread_ = std::thread(&MusicPlayer::playerLoop, this, playlist);
    }

    void MusicPlayer::stop() {
        if (!running_) return;

        running_ = false;

        if (playerThread_.joinable()) {
            playerThread_.join();
        }
    }

    void MusicPlayer::playerLoop(Playlist playlist) {

        if (playlist.empty()) {
            logger().warn("Playlist is empty.");
            return;
        }

        if (playlist.size() == 1) {
            playSingleTrackLooped(playlist[0]);
        } else {
            playPlaylistWithCrossfade(playlist);
        }

        running_ = false; // Ensure the running flag is reset when playback ends
        logger().warn("Music playback stopped.");
    }

    void MusicPlayer::playSingleTrackLooped(const Track& track) {
        logger().info("Playing single track in loop: {}", track.string());

        AudioStream stream = BASS_StreamCreateFile(FALSE, track.c_str(), 0, 0, BASS_SAMPLE_LOOP);
        if (!stream) {
            logger().error("Failed to play single track. Error code = {}", BASS_ErrorGetCode());;
            return;
        }

        BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, 0);
        BASS_ChannelPlay(stream, FALSE);
        std::thread fadeInThread(&MusicPlayer::fadeIn, this, stream);
        fadeInThread.join();

        while (running_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        cleanupStream(stream);
    }

    void MusicPlayer::playPlaylistWithCrossfade(const Playlist& playlist) {
        logger().info("Playing playlist with crossfade.");

        const auto currentResult = findNextPlayableTrack(playlist, 0);
        if (!currentResult) {
            logger().error("No playable tracks found in playlist.");
            return;
        }

        size_t currentIndex = currentResult->first;
        AudioStream current = currentResult->second;

        std::thread fadeInThread(&MusicPlayer::fadeIn, this, current);
        fadeInThread.join();

        do {
            logger().debug("Playing track: {}", playlist[currentIndex].string());

            waitBeforeTransition(current);
            if (!running_) break;

            auto nextResult = findNextPlayableTrack(playlist, (currentIndex + 1) % playlist.size());
            if (!nextResult) {
                logger().error("No playable next track found. Stopping playback.");
                break;
            }

            size_t nextIndex = nextResult->first;
            AudioStream next = nextResult->second;

            crossfade(current, next);
            cleanupStream(current);

            current = next;
            currentIndex = nextIndex;

        } while (running_);

        cleanupStream(current);
    }

    AudioStream MusicPlayer::playTrack(const Track& track) {
        const AudioStream stream = BASS_StreamCreateFile(FALSE, track.c_str(), 0, 0, BASS_SAMPLE_LOOP);

        if (stream) {
            logger().debug("Successfully opened audio stream: {}", track.string());

            BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, 0);
            BASS_ChannelPlay(stream, FALSE);
        }else {
            logger().error("Failed to open audio stream: {}. Error code = {}", track.string(), BASS_ErrorGetCode());
        }
        return stream;
    }

    std::optional<std::pair<size_t, AudioStream>> MusicPlayer::findNextPlayableTrack(
        const Playlist& playlist, size_t startIndex
    ) {
        size_t index {startIndex};
        size_t attempts {0};

        while (attempts < playlist.size() && running_) {
            AudioStream stream = playTrack(playlist[index]);
            if (stream) {
                return std::make_pair(index, stream);
            }

            logger().warn("Skipping unplayable track: {}", playlist[index].string());
            index = (index + 1) % playlist.size(); // wrap around to the start
            ++attempts;
        }

        return {}; // No valid track found
    }

    void MusicPlayer::waitBeforeTransition(const AudioStream stream) {
        const AudioPosition length = BASS_ChannelGetLength(stream, BASS_POS_BYTE);
        const double totalSecs = BASS_ChannelBytes2Seconds(stream, length);

        // while the current position is less than totalSecs - 5 seconds
        while (running_
        && BASS_ChannelBytes2Seconds(stream, BASS_ChannelGetPosition(stream, BASS_POS_BYTE)) < totalSecs - 5.0)
        {
            // wait and release when we are close to the end of the track
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    void MusicPlayer::fadeVolume(AudioChannel channel, float start, float end, float duration) {
        if (!channel) return;

        constexpr int steps = 20;
        for (int i = 0; i <= steps && running_; ++i) {

            const float vol = start + (end - start) * (static_cast<float>(i) / steps);
            BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, vol);

            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(duration * 1000 / steps)));
        }
    }

    void MusicPlayer::fadeIn(AudioChannel channel) {
        fadeVolume(channel, 0.0f, 1.0f, FADE_DURATION);
    }

    void MusicPlayer::fadeOut(AudioChannel channel) {
        fadeVolume(channel, 1.0f, 0.0f, FADE_DURATION);
    }

    void MusicPlayer::crossfade(AudioStream current, AudioStream next) {
        std::thread fadeOutThread(&MusicPlayer::fadeOut, this, current);
        std::thread fadeInThread(&MusicPlayer::fadeIn, this, next);

        fadeOutThread.join();
        fadeInThread.join();
    }

    void MusicPlayer::cleanupStream(AudioStream stream) {
        if (stream) {
            BASS_ChannelStop(stream);
            BASS_StreamFree(stream);
        }
    }

    bool MusicPlayer::isPlayable(const fs::path &path) {
        const AudioStream stream = BASS_StreamCreateFile(FALSE, path.c_str(), 0, 0, BASS_SAMPLE_LOOP);
        if (!stream) return false;

        BASS_StreamFree(stream);
        return true;
    }

    bool MusicPlayer::hasAtLeastOnePlayable(const Playlist& playlist) {
        return std::ranges::any_of(playlist, [](const fs::path& path) {
            return isPlayable(path);
        });
    }

    MusicPlayer::Playlist MusicPlayer::loadPlaylist(const fs::path& folder) {
        Playlist files;

        if (!fs::exists(folder)) {
            return files;
        }

        for (const auto& entry : fs::directory_iterator(folder)) {
            const auto& path = entry.path();
            if (path.extension() == ".mp3" || path.extension() == ".wav") {
                files.push_back(path);
            }
        }

        return files;
    }

    void MusicPlayer::shufflePlaylist(Playlist& playlist) {
        std::random_device rd;
        std::mt19937 g{rd()};
        std::ranges::shuffle(playlist, g);
    }

} // namespace PiAlarm::media
