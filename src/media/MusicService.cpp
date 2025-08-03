#include "media/MusicService.h"

namespace PiAlarm::media {

    MusicService::MusicService(const fs::path& folder, const fs::path& fallbackFolder)
        : HasLogger{"Media::MusicService"}, folder_{folder}, fallbackFolder_{fallbackFolder}
    {}

    void MusicService::start() {
        auto playlist = MusicPlayer::loadPlaylist(folder_);

        if (!MusicPlayer::hasAtLeastOnePlayable(playlist)) {
            logger().warn("No valid tracks found in primary folder ({}), falling back to {}", folder_.string(), fallbackFolder_.string());
            playlist = MusicPlayer::loadPlaylist(fallbackFolder_);

            if (!MusicPlayer::hasAtLeastOnePlayable(playlist)) {
                logger().error("No valid tracks found in both folders.");
                return;
            }
        }

        MusicPlayer::shufflePlaylist(playlist);

        musicPlayer_.start(playlist);
    }

} // namespace PiAlarm::media
