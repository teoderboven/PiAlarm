#include "media/MusicService.h"
#include <filesystem>
#include <algorithm>

namespace PiAlarm::media {

    MusicService::MusicService(const fs::path& folder, const fs::path& fallbackFolder)
        : HasLogger{"Media::MusicService"}, folder_{folder}, fallbackFolder_{fallbackFolder}
    {}

    void MusicService::start() {
        auto playlist = loadPlaylist(folder_);

        if (!atLeastOnePlayable(playlist)) {
            logger().warn("No valid tracks found in primary folder, falling back to: {}", fallbackFolder_.string());
            playlist = loadPlaylist(fallbackFolder_);

            if (!atLeastOnePlayable(playlist)) {
                logger().error("No valid tracks found in both folders.");
                return;
            }
        }

        musicPlayer_.start(playlist);
    }

    MusicPlayer::Playlist MusicService::loadPlaylist(const fs::path& folder) const {
        MusicPlayer::Playlist files;

        if (!fs::exists(folder)) {
            logger().error("Folder {} does not exist.", folder.string());
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

    bool MusicService::atLeastOnePlayable(const MusicPlayer::Playlist& playlist) {
        return std::ranges::any_of(playlist, [](const fs::path& path) {
            return MusicPlayer::isPlayable(path);
        });
    }

} // namespace PiAlarm::media
