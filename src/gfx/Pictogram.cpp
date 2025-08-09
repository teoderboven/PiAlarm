#include <stdexcept>
#include "lodepng.h"

#include "gfx/Types.h"
#include "gfx/Pictogram.h"

namespace PiAlarm::gfx {

    void Pictogram::loadAndConvert(const std::string& filename) {
        std::vector<uint8_t> image;
        unsigned width, height;

        // Load the PNG image
        auto error = lodepng::decode(image, width, height, filename);
        if (error) {
            throw std::runtime_error("Failed to load PNG image: " + filename + " - " + lodepng_error_text(error));
        }

        // Convert to grayscale and create Bitmap
        bmp_ = Bitmap(width, height);
        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {

                size_t index = (y * width + x) * 4; // RGBA
                uint8_t gray = rgbToGray(image[index], image[index + 1], image[index + 2]);

                bmp_.setPixel(x, y, gray);
            }
        }
    }

} // namespace PiAlarm::gfx
