#ifndef PIALARM_VIEWFORMATUTILS_H
#define PIALARM_VIEWFORMATUTILS_H

#include <string>
#include <sstream>
#include <iomanip>

#include "model/Time.h"
#include "utils/WeatherUtils.hpp"

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
    inline std::string formatTime(model::Time time, bool displayTime = true, bool includeSeconds = false) {
        return displayTime ? time.toString(includeSeconds) : includeSeconds ? "--:--:--" : "--:--";
    }

    /**
     * @brief Formats the temperature for display.
     *
     * If the temperature data is valid, it returns the formatted temperature value with "°C" suffix.
     * If not valid, it returns a placeholder string.
     *
     * @param temperature The temperature value to format.
     * @param valid Indicates whether the temperature data is valid or not.
     * @return A formatted string representing the temperature value or a placeholder if invalid.
     */
    inline std::string formatTemperature(float temperature, bool valid) {
        return formatValue(temperature, valid, 1, "°C", "--.-°C");
    }

    /**
     * @brief Formats the humidity for display.
     *
     * If the humidity data is valid, it returns the formatted humidity value with a percentage sign.
     * If not valid, it returns a placeholder string.
     *
     * @param humidity The humidity value to format.
     * @param valid Indicates whether the humidity data is valid or not.
     * @return A formatted string representing the humidity value or a placeholder if invalid.
     */
    inline std::string formatHumidity(float humidity, bool valid) {
        return formatValue(humidity, valid, 0, "%", "--%");
    }

    /**
     * @brief Formats the atmospheric pressure for display.
     *
     * If the pressure data is valid, it returns the formatted pressure value with " hPa" suffix.
     * If not valid, it returns a placeholder string.
     *
     * @param pressure The atmospheric pressure value to format.
     * @param valid Indicates whether the pressure data is valid or not.
     * @return A formatted string representing the pressure value or a placeholder if invalid.
     */
    inline std::string formatPressure(float pressure, bool valid) {
        return formatValue(pressure, valid, 1, " hPa", "----.- hPa");
    }

    /**
     * @brief Formats the weather condition for display.
     *
     * If the weather condition is valid, it returns a localized string representation of the condition.
     * If not valid, it returns "???" as a placeholder.
     *
     * @param condition The weather condition to format.
     * @param valid Indicates whether the weather data is valid or not.
     * @param locale The locale for the weather condition string (default is "fr").
     * @return A formatted string representing the weather condition or a placeholder if invalid.
     */
    inline std::string formattedWeatherCondition(common::WeatherCondition condition, bool valid, const std::string& locale = "fr") {
        if (!valid) return "???";

        return getLocalizedWeatherCondition(condition, locale);
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

#endif //PIALARM_VIEWFORMATUTILS_H
