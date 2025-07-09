#ifndef MAINCLOCKVIEW_H
#define MAINCLOCKVIEW_H

#include "view/cli/BaseCliView.h"
#include "model/AlarmData.hpp"
#include "model/ClockData.hpp"
#include "model/WeatherData.hpp"

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
        const model::AlarmData& alarmData_;     ///< Reference to the alarm data model
        const model::ClockData& clockData_;     ///< Reference to the clock data model
        const model::WeatherData& weatherData_; ///< Reference to the weather data model

        // Current state variables
        model::Time currentTime_; ///< Current time
        model::Time alarmTime_;   ///< Current alarm time
        bool alarmEnabled_;       ///< Flag indicating if the alarm is enabled
        float temperature_;       ///< Current temperature
        float humidity_;          ///< Current humidity
        bool weatherValid_;       ///< Flag indicating if the weather data is valid

    public:

        /**
         * @brief Constructor for MainClockView.
         * @param alarmData Reference to the alarm data model.
         * @param clockData Reference to the clock data model.
         * @param weatherData Reference to the weather data model.
         */
        MainClockView(const model::AlarmData& alarmData, const model::ClockData& clockData, const model::WeatherData& weatherData);

        // Inherited from IView
        void refresh() override;
        void render(DisplayType& display) override;

    private:
        /**
         * Gives the current time in a formatted string.
         * @return Formatted current time as a string.
         */
        inline std::string formattedCurrentTime() const;

        /**
         * Gives the alarm time in a formatted string.
         * @return Formatted alarm time as a string.
         */
        inline std::string formattedAlarmTime() const;

        /**
         * Gives the temperature in a formatted string.
         * @return Formatted temperature as a string.
         */
        inline std::string formattedTemperature() const;

        /**
         * Gives the humidity in a formatted string.
         * @return Formatted humidity as a string.
         */
        inline std::string formattedHumidity() const;

    };

} // namespace PiAlarm::view::cli

#endif //MAINCLOCKVIEW_H
