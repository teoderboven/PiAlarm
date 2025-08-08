#ifndef MAINCLOCKVIEW_H
#define MAINCLOCKVIEW_H

#include <string>
#include <vector>
#include <utility>

#include "view/AbstractMainClockView.h"

/**
 * @namespace PiAlarm::view::console
 * @brief Namespace for console views in the PiAlarm application.
 *
 * This namespace contains classes and functions related to the console views of the PiAlarm application.
 */
namespace PiAlarm::view::console {

    /**
     * @class MainClockView
     * @brief Console view for displaying the main clock information.
     *
     * This class implements the main clock view for the console, displaying the current time,
     * alarm status, and weather information in a formatted manner.
     */
    class MainClockView final : public AbstractMainClockView {
        public:

        /**
         * @brief Constructor for MainClockView.
         * @param alarmsData Reference to the alarms data model.
         * @param alarmStateData Reference to the alarm state data model.
         * @param clockData Reference to the clock data model.
         * @param currentWeatherData Reference to the current weather data model.
         * @param temperatureSensorData Reference to the temperature sensor data model.
         */
        MainClockView(
            const model::AlarmsData& alarmsData,
            const model::AlarmState& alarmStateData,
            const model::ClockData& clockData,
            const model::CurrentWeatherData& currentWeatherData,
            const model::TemperatureSensorData& temperatureSensorData
        );

        /**
         * @brief Destructor for MainClockView.
         * Cleans up resources and stops observing the data models.
         */
        ~MainClockView() override = default;

        /**
         * @brief Renders the view using the provided renderer.
         * This method displays the current time, alarm status, and weather information in the console.
         * @param renderer The renderer used to output the view.
         */
        void render(RenderType& renderer) override;

    private:

        /**
         * @brief Displays labels and their corresponding values.
         * This method formats and displays a list of labels and their values in the CLI.
         * @param renderer The renderer used to output the labels.
         * @param labels A vector of pairs containing label names and their corresponding values.
         */
        void displayLabels(RenderType& renderer, const std::vector<std::pair<std::string, std::string>>& labels);

        /**
         * @brief Gets the current alarm status.
         * This method checks if there is an enabled alarm and returns a string representation of the alarm status.
         * @return A string indicating the current alarm status.
         */
        [[nodiscard]]
        std::string getAlarmStatus() const;

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

} // namespace PiAlarm::view::console

#endif //MAINCLOCKVIEW_H
