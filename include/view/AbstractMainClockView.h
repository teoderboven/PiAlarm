#ifndef PIALARM_ABSTRACTMAINCLOCKVIEW_H
#define PIALARM_ABSTRACTMAINCLOCKVIEW_H

#include "view/AbstractObserverView.h"
#include "model/AlarmsData.hpp"
#include "model/AlarmState.hpp"
#include "model/ClockData.hpp"
#include "model/CurrentWeatherData.h"
#include "model/TemperatureSensorData.hpp"

namespace PiAlarm::view {

    /**
     * @class AbstractMainClockView
     * @brief Abstract base class for the main clock view.
     *
     * This class provides a base implementation for the main clock view, which displays the current time,
     * alarm information, and weather data.
     */
    class AbstractMainClockView : public AbstractObserverView {
    protected:
        const model::AlarmsData& alarmsData_; ///< Reference to the alarms data model
        const model::AlarmState& alarmStateData_; ///< Reference to the alarm state data model
        const model::ClockData& clockData_; ///< Reference to the clock data model
        const model::CurrentWeatherData& currentWeatherData_; ///< Reference to the current weather data model
        const model::TemperatureSensorData& temperatureSensorData_; ///< Reference to the temperature sensor data model

        // Current state variables
        model::Time currentTime_; ///< Current time

        size_t enabledAlarmCount_; ///< Number of enabled alarms
        model::Time nextAlarmTime_;  ///< Next alarm time
        bool hasAlarmEnabled_;       ///< Flag indicating if there is an enabled alarm

        float currentIndoorTemperature_; ///< Current indoor temperature
        float currentIndoorHumidity_;    ///< Current indoor humidity
        bool sensorDataValid_; ///< Flag indicating if the temperature sensor data is valid

        float currentOutdoorTemperature_; ///< Current outdoor temperature
        float currentOutdoorHumidity_;    ///< Current outdoor humidity
        float currentOutdoorPressure_;    ///< Current outdoor pressure
        common::WeatherCondition currentWeatherCondition_; ///< Current weather condition
        bool currentWeatherDataValid_; ///< Flag indicating if the current weather data is valid

    public:

        /**
         * @brief Constructor for AbstractMainClockView.
         * @param alarmsData Reference to the alarms data model.
         * @param alarmStateData Reference to the alarm state data model.
         * @param clockData Reference to the clock data model.
         * @param currentWeatherData Reference to the current weather data model.
         * @param temperatureSensorData Reference to the temperature sensor data model.
         */
        AbstractMainClockView(
            const model::AlarmsData& alarmsData,
            const model::AlarmState& alarmStateData,
            const model::ClockData& clockData,
            const model::CurrentWeatherData& currentWeatherData,
            const model::TemperatureSensorData& temperatureSensorData
        );

        /**
         * @brief Destructor for AbstractMainClockView.
         * Cleans up resources and stops observing the data models.
         */
        ~AbstractMainClockView() override;

        // Inherited from IView

        /**
         * @brief Refreshes the view by updating the current time and other state variables.
         */
        void refresh() override;

        /**
         * @brief Renders the view using the provided renderer.
         * This method should be implemented by derived classes to display the current time,
         * alarm information, and weather data.
         *
         * @param renderer The renderer to use for displaying the view.
         */
        virtual void render(RenderType& renderer) const override = 0;

    };

} // namespace PiAlarm::view

#endif //PIALARM_ABSTRACTMAINCLOCKVIEW_H
