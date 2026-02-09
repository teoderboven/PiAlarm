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
     * @brief Concrete implementation of the main clock view for the console.
     *
     * This class extends AbstractMainClockView and provides the rendering logic for the main clock view,
     * including displaying the current time, alarm information, and weather data in a console format.
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
            const model::CurrentIndoorData& temperatureSensorData
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
        void render(RenderType& renderer) const override;

    private:

        /**
         * @brief Displays labels and their corresponding values.
         * This method formats and displays a list of labels and their values in the CLI.
         * @param renderer The renderer used to output the labels.
         * @param labels A vector of pairs containing label names and their corresponding values.
         */
        void displayLabels(RenderType& renderer, const std::vector<std::pair<std::string, std::string>>& labels) const;

        /**
         * @brief Gets the current alarm status.
         * This method checks if there is an enabled alarm and returns a string representation of the alarm status.
         * @return A string indicating the current alarm status.
         */
        [[nodiscard]]
        std::string getAlarmStatus() const;

    };

} // namespace PiAlarm::view::console

#endif //MAINCLOCKVIEW_H
