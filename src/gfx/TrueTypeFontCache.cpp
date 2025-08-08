#include "gfx/TrueTypeFontCache.h"
#include "gfx/TrueTypeFont.h"

namespace PiAlarm::gfx {

    // Initialize the static members of TrueTypeFontCache
    std::unordered_map<std::string, std::shared_ptr<IFont>> TrueTypeFontCache::cache_;
    std::mutex TrueTypeFontCache::cacheMutex_;

    std::shared_ptr<IFont> TrueTypeFontCache::getFont(const std::string& fontPath, int pixelHeight) {
        std::lock_guard lock{cacheMutex_};

        const auto key {makeKey(fontPath, pixelHeight)};

        // search in cache
        auto it {cache_.find(key)};
        if (it != cache_.end()) {
            return it->second;
        }

        // not found in cache, create a new TrueTypeFont
        auto font {std::make_shared<TrueTypeFont>(fontPath, pixelHeight)};
        cache_[key] = font;

        return font;
    }

    std::string TrueTypeFontCache::makeKey(const std::string& fontPath, int pixelHeight) {
        return fontPath + ":" + std::to_string(pixelHeight);
    }

} // namespace PiAlarm::gfx
