#ifndef PIALARM_PICTOGRAM_H
#define PIALARM_PICTOGRAM_H

#include "gfx/Bitmap.h"

namespace PiAlarm::gfx {

    /**
     * @class Pictogram
     * @brief Represents a pictogram image loaded from a PNG file.
     *
     * This class loads a PNG image, converts it to grayscale, and provides access to the bitmap data.
     */
    class Pictogram {
        Bitmap bmp_; ///< The bitmap representation of the pictogram image

    public:

        /**
         * @brief Constructs a Pictogram from a PNG file.
         * @param filename The path to the PNG file to load.
         * @throws std::runtime_error if the image cannot be loaded or converted.
         */
        inline explicit Pictogram(const std::string& filename);

        /**
         * @brief Gets the bitmap representation of the pictogram.
         * @return A reference to the Bitmap object containing the pictogram data.
         */
        inline const Bitmap& getBitmap() const;

        /**
         * @brief Gets the width of the pictogram.
         * @return The width of the pictogram in pixels.
         */
        inline size_t getWidth() const;

        /**
         * @brief Gets the height of the pictogram.
         * @return The height of the pictogram in pixels.
         */
        inline size_t getHeight() const;

    private:

        /**
         * @brief Loads a PNG image from a file and converts it to grayscale.
         * @param filename The path to the PNG file to load.
         * @throws std::runtime_error if the image cannot be loaded or converted.
         */
        void loadAndConvert(const std::string& filename);

        /**
         * @brief Converts RGB values to a grayscale pixel value.
         * @param r The red component (0-255).
         * @param g The green component (0-255).
         * @param b The blue component (0-255).
         * @return The grayscale pixel value (0-255).
         */
        inline static Pixel rgbToGray(uint8_t r, uint8_t g, uint8_t b);

    };

    // Inline methods implementation

    inline Pictogram::Pictogram(const std::string& filename) {
        loadAndConvert(filename);
    }

    inline const Bitmap& Pictogram::getBitmap() const {
        return bmp_;
    }

    inline size_t Pictogram::getWidth() const {
        return bmp_.width;
    }

    inline size_t Pictogram::getHeight() const {
        return bmp_.height;
    }

    inline Pixel Pictogram::rgbToGray(uint8_t r, uint8_t g, uint8_t b) {
        return static_cast<Pixel>(0.299 * r + 0.587 * g + 0.114 * b);
    }

} // namespace PiAlarm::gfx

#endif //PIALARM_PICTOGRAM_H
