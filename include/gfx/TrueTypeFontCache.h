#ifndef PIALARM_TTFONTCACHE_H
#define PIALARM_TTFONTCACHE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>

#include "gfx/IFont.h"

// available font paths
#define FONT_MozillaText_Regular "assets/fonts/MozillaText-Regular.ttf"
#define FONT_MozillaText_Light "assets/fonts/MozillaText-Light.ttf"

namespace PiAlarm::gfx {

    /**
     * @class TrueTypeFontCache
     * @brief A cache for TrueType fonts to avoid loading the same font multiple times.
     *
     * This class provides a static method to retrieve a TrueType font from the cache,
     * creating it if it does not already exist. It uses a mutex to ensure thread safety.
     */
    class TrueTypeFontCache {
        static std::unordered_map<std::string, std::shared_ptr<IFont>> cache_; ///< Cache for TrueType fonts, mapping font paths and pixel heights to font objects
        static std::mutex cacheMutex_; ///< Mutex to protect access to the cache

    public:

        /**
         * @brief Deleted default constructor to prevent instantiation.
         * This class is intended to be used statically, so it does not need an instance.
         */
        TrueTypeFontCache() = delete;

        /**
         * @brief Retrieves a TrueType font from the cache or creates it if not found.
         * @param fontPath The path to the TrueType font file.
         * @param pixelHeight The height of the font in pixels.
         * @return A shared pointer to the TrueTypeFont object.
         */
        static std::shared_ptr<IFont> getFont(const std::string& fontPath, int pixelHeight);

    private:

        /**
         * @brief Generates a unique key for the font based on its path and pixel height.
         * This key is used to store and retrieve the font from the cache.
         * @param fontPath The path to the TrueType font file.
         * @param pixelHeight The height of the font in pixels.
         * @return A string representing the unique key for the font.
         */
        static std::string makeKey(const std::string& fontPath, int pixelHeight);

    };

} // namespace PiAlarm::gfx

#endif //PIALARM_TTFONTCACHE_H
