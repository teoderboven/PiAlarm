#ifndef PIALARM_VIEWUTILS_H
#define PIALARM_VIEWUTILS_H

#include <string>
#include <sstream>
#include <iomanip>

#include "model/Time.h"

namespace PiAlarm::utils {

    /**
     * @brief Formats a floating-point value with fixed precision and a unit suffix.
     *
     * If the value is invalid (`valid` is false), a placeholder string is returned instead.
     *
     * @param value The floating-point value to format.
     * @param valid Indicates whether the value is valid or not.
     * @param precision The number of decimal places to include.
     * @param unit The unit string to append after the value (e.g., "°C", "%").
     * @param placeholder The string to return if the value is not valid.
     * @return A formatted string representing the value with the specified precision and unit,
     *         or the placeholder if the value is invalid.
     */
    inline std::string formatValue(float value, bool valid, int precision, const std::string& unit, const std::string& placeholder) {
        if (!valid) return placeholder;

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << value << unit;
        return oss.str();
    }

    /**
     * @brief Formats a floating-point value with fixed precision, without a unit suffix.
     *
     * If the value is invalid (`valid` is false), a placeholder string is returned instead.
     * This overload is equivalent to calling formatValue with an empty unit string.
     *
     * @param value The floating-point value to format.
     * @param valid Indicates whether the value is valid or not.
     * @param precision The number of decimal places to include.
     * @param placeholder The string to return if the value is not valid.
     * @return A formatted string representing the value with the specified precision,
     *         or the placeholder if the value is invalid.
     */
    inline std::string formatValue(float value, bool valid, int precision, const std::string& placeholder) {
        return formatValue(value, valid, precision, "", placeholder);
    }

    /**
     * @brief Formats a model::Time object into a string representation.
     *
     * If `displayTime` is true, the time is formatted as a string with or without seconds based on `includeSeconds`.
     * If `displayTime` is false, a placeholder string is returned.
     *
     * @param time The model::Time object to format.
     * @param displayTime Indicates whether to display the time or return a placeholder.
     * @param includeSeconds Indicates whether to include seconds in the formatted time.
     * @return A formatted string representing the time or a placeholder if not displaying time.
     */
    inline std::string formattedTime(model::Time time, bool displayTime = true, bool includeSeconds = false) {
        return displayTime ? time.toString(includeSeconds) : includeSeconds ? "--:--:--" : "--:--";
    }

    /**
     * @brief Formats an integer value with leading zeros to a specified minimum number of digits.
     *
     * This function converts the integer to a string, ensuring it has at least `minDigits` digits,
     * padding with leading zeros if necessary.
     *
     * @param value The integer value to format.
     * @param minDigits The minimum number of digits in the formatted string.
     * @return A string representation of the integer with leading zeros as needed.
     */
    inline std::string formatInt(int value, int minDigits) {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(minDigits) << value;
        return oss.str();
    }

} // namespace PiAlarm::utils

#endif //PIALARM_VIEWUTILS_H
