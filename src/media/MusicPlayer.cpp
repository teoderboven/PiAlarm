#include "media/MusicPlayer.h"
#include <filesystem>
#include <random>
#include <chrono>

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
    }

    void MusicPlayer::playSingleTrackLooped(const Track& track) {
        logger().info("Playing single track in loop: {}", track.string());

        AudioStream stream = BASS_StreamCreateFile(FALSE, track.c_str(), 0, 0, BASS_SAMPLE_LOOP);
        if (!stream) {
            logger().error("Failed to play single track.");
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
        size_t currentIndex = 0;

        logger().info("Playing playlist with crossfade.");

        AudioStream current = playTrack(playlist[currentIndex]);

        std::thread fadeInThread(&MusicPlayer::fadeIn, this, current);
        fadeInThread.join();

        do {
            logger().debug("Playing track: {}", playlist[currentIndex].string());

            waitBeforeTransition(current);
            if (!running_) break;

            currentIndex = (currentIndex + 1) % playlist.size();
            AudioStream next = playTrack(playlist[currentIndex]);
            if (!next) break;

            crossfade(current, next);
            cleanupStream(current);
            current = next;

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
            logger().error("Failed to open audio stream: {}. Error code: {}", track.string(), BASS_ErrorGetCode());
        }
        return stream;
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

    bool MusicPlayer::isPlayable(const std::filesystem::path &path) {
        const AudioStream stream = BASS_StreamCreateFile(FALSE, path.c_str(), 0, 0, BASS_SAMPLE_LOOP);
        if (!stream) return false;

        BASS_StreamFree(stream);
        return true;
    }

} // namespace PiAlarm::media
