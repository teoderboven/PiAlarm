#ifndef CONSOLEDISPLAYUTILS_HPP
#define CONSOLEDISPLAYUTILS_HPP

#include <string>


namespace PiAlarm::utils {

    /**
     * @brief Counts the number of multibyte characters in a UTF-8 encoded string.
     *
     * This function iterates through the string and counts characters based on their byte representation.
     * It correctly handles ASCII characters (1 byte) and multibyte UTF-8 characters (2 to 4 bytes).
     *
     * @param str The input string to count multibyte characters in.
     * @return The count of multibyte characters in the string.
     */
    inline size_t countMultibyteChars(const std::string& str) {
        size_t count = 0;

        for (size_t i = 0; i < str.size(); ) {
            unsigned char c = static_cast<unsigned char>(str[i]);

            if ((c & 0x80) == 0x00) {
                // ASCII (1 byte)
                i += 1;
            } else if ((c & 0xE0) == 0xC0) {
                // 2-byte character
                ++count;
                i += 2;
            } else if ((c & 0xF0) == 0xE0) {
                // 3-byte character
                ++count;
                i += 3;
            } else if ((c & 0xF8) == 0xF0) {
                // 4-byte character (emoji, etc.)
                ++count;
                i += 4;
            } else {
                // Invalid UTF-8 or continuation byte: skip 1
                i += 1;
            }
        }

        return count;
    }

} // namespace PiAlarm::utils

#endif //CONSOLEDISPLAYUTILS_HPP
