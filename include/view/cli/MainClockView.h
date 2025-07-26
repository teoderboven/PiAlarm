#ifndef MAINCLOCKVIEW_H
#define MAINCLOCKVIEW_H

#include "view/cli/BaseCliView.h"
#include "model/AlarmsData.hpp"
#include "model/ClockData.hpp"
#include "model/CurrentWeatherData.h"
#include "model/TemperatureSensorData.hpp"

/**
 * @namespace PiAlarm::view::cli
 * @brief Namespace for CLI views in the PiAlarm application.
 *
 * This namespace contains classes that represent different views in the command-line interface of the PiAlarm application.
 */
namespace PiAlarm::view::cli {

    /**
     * @class MainClockView
     * @brief Represents the main clock view in the CLI application.
     *
     * This class is responsible for displaying the current time, alarm time, and temperature.
     */
    class MainClockView final : public BaseCliView {
        const model::AlarmsData& alarmsData_; ///< Reference to the alarms data model
        const model::ClockData& clockData_; ///< Reference to the clock data model
        const model::CurrentWeatherData& currentWeatherData_; ///< Reference to the current weather data model
        const model::TemperatureSensorData& temperatureSensorData_; ///< Reference to the temperature sensor data model

        // Current state variables
        model::Time currentTime_; ///< Current time
        model::Time nextAlarmTime_;   ///< Next alarm time
        bool hasAlarmEnabled_;       ///< Flag indicating if there is an enabled alarm

        float currentIndoorTemperature_; ///< Current indoor temperature
        float currentIndoorHumidity_;    ///< Current indoor humidity
        bool sensorDataValid_;     ///< Flag indicating if the temperature sensor data is valid

        float currentOutdoorTemperature_; ///< Current outdoor temperature
        float currentOutdoorHumidity_;    ///< Current outdoor humidity
        float currentOutdoorPressure_; ///< Current outdoor pressure
        common::WeatherCondition currentWeatherCondition_; ///< Current weather condition
        bool currentWeatherDataValid_; ///< Flag indicating if the current weather data is valid

    public:

        /**
         * @brief Constructor for MainClockView.
         * @param alarmsData Reference to the alarms data model.
         * @param clockData Reference to the clock data model.
         * @param currentWeatherData Reference to the current weather data model.
         * @param temperatureSensorData Reference to the temperature sensor data model.
         */
        MainClockView(
            const model::AlarmsData& alarmsData,
            const model::ClockData& clockData,
            const model::CurrentWeatherData& currentWeatherData,
            const model::TemperatureSensorData& temperatureSensorData
        );

        // Inherited from IView
        void refresh() override;
        void render(DisplayType& display) override;

    private:

        /**
         * @brief Formats a value for display.
         * @param value The value to format.
         * @param valid Flag indicating whether the value is valid.
         * @param precision The number of decimal places to include in the formatted string.
         * @param unit The unit of measurement for the value (e.g., "°C", "%").
         * @param placeholder The placeholder string to use if the value is not valid.
         * @return Formatted string representation of the value.
         */
        static std::string formatValue(float value, bool valid, int precision, const std::string& unit, const std::string& placeholder);

        /**
         * @brief Formats the time for display.
         * @param time The time to format.
         * @param displayTime Flag indicating whether to display the time or a placeholder.
         * @return Formatted time string.
         */
        static std::string formattedTime(const model::Time& time, bool displayTime = true);

        /**
         * @brief Formats the temperature for display.
         * @param temperature The temperature to format.
         * @param valid Flag indicating whether the temperature data is valid.
         * @return Formatted temperature string.
         */
        static std::string formattedTemperature(float temperature, bool valid);

        /**
         * @brief Formats the humidity for display.
         * @param humidity The humidity to format.
         * @param valid Flag indicating whether the humidity data is valid.
         * @return Formatted humidity string.
         */
        static std::string formattedHumidity(float humidity, bool valid);

        /**
         * @brief Formats the pressure for display.
         * @param pressure The pressure to format.
         * @param valid Flag indicating whether the pressure data is valid.
         * @return Formatted pressure string.
         */
        static std::string formattedPressure(float pressure, bool valid);

        /**
         * @brief Formats the weather condition for display.
         * @param condition The weather condition to format.
         * @param valid Flag indicating whether the weather data is valid.
         * @param locale The locale for the weather condition string (default is "fr").
         * @return Formatted weather condition string.
         */
        static std::string formattedWeatherCondition(common::WeatherCondition condition, bool valid, const std::string& locale = "fr");

    };

} // namespace PiAlarm::view::cli

#endif //MAINCLOCKVIEW_H
