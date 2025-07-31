#include "media/MusicPlayer.h"
#include <filesystem>
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>
#include <utility>

namespace PiAlarm::media {

    namespace fs = std::filesystem;

    MusicPlayer::MusicPlayer(std::string folderPath)
        : folderPath_(std::move(folderPath)), running_(false)
    {}

    MusicPlayer::~MusicPlayer() {
        stop();
    }

    void MusicPlayer::start() {
        if (running_) return;

        running_ = true;
        playerThread_ = std::thread(&MusicPlayer::playerLoop, this);
    }

    void MusicPlayer::stop() {
        if (!running_) return;

        running_ = false;

        if (playerThread_.joinable()) {
            playerThread_.join();
        }
    }

    void MusicPlayer::playerLoop() {
        loadPlaylist();

        if (playlist_.empty()) {
            std::cerr << "Aucun fichier audio trouvé.\n";
            return;
        }

        if (playlist_.size() == 1) {
            playSingleTrackLooped();
        } else {
            playPlaylistWithCrossfade();
        }
    }

    void MusicPlayer::playSingleTrackLooped() {
        AudioStream stream = BASS_StreamCreateFile(FALSE, playlist_[0].c_str(), 0, 0, BASS_SAMPLE_LOOP);
        if (!stream) {
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

    void MusicPlayer::playPlaylistWithCrossfade() {
        size_t currentIndex = 0;

        AudioStream current = playTrack(currentIndex);

        std::thread fadeInThread(&MusicPlayer::fadeIn, this, current);
        fadeInThread.join();

        do {
            waitBeforeTransition(current);
            if (!running_) break;

            currentIndex = (currentIndex + 1) % playlist_.size();
            AudioStream next = playTrack(currentIndex);
            if (!next) break;

            crossfade(current, next);
            cleanupStream(current);
            current = next;

        } while (running_);

        cleanupStream(current);
    }

    void MusicPlayer::loadPlaylist() {
        playlist_ = getFiles(folderPath_);
        std::ranges::shuffle(playlist_, std::mt19937{std::random_device{}()});
    }

    AudioStream MusicPlayer::playTrack(size_t index) {
        if (index >= playlist_.size()) return 0;
        AudioStream stream = BASS_StreamCreateFile(FALSE, playlist_[index].c_str(), 0, 0, BASS_SAMPLE_LOOP);
        if (stream) {
            BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, 0);
            BASS_ChannelPlay(stream, FALSE);
        }
        return stream;
    }

    void MusicPlayer::waitBeforeTransition(AudioStream stream) {
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

            const float vol = start + (end - start) * (i / static_cast<float>(steps));
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

    std::vector<std::string> MusicPlayer::getFiles(const std::string& folder) {
        std::vector<std::string> files;
        for (const auto& entry : fs::directory_iterator(folder)) {
            if (entry.path().extension() == ".mp3" || entry.path().extension() == ".wav") {
                files.push_back(entry.path().string());
            }
        }
        return files;
    }

} // namespace PiAlarm::media
