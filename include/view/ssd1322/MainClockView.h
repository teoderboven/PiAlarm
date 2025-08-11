#ifndef PIALARM_MAINCLOCKVIEW_H
#define PIALARM_MAINCLOCKVIEW_H

#include "view/AbstractMainClockView.h"
#include "gfx/TrueTypeFont.h"
#include "gfx/TrueTypeFontCache.h"
#include "gfx/Pictogram.h"

/**
 * @namespace PiAlarm::view::ssd1322
 * @brief Namespace for the SSD1322 display views in the PiAlarm application.
 *
 * This namespace contains classes related to the SSD1322 display views.
 */
namespace PiAlarm::view::ssd1322 {

    /**
     * @class MainClockView
     * @brief Concrete implementation of the main clock view for the SSD1322 display.
     *
     * This class extends AbstractMainClockView and provides the rendering logic for the main clock view,
     * including displaying the current time, alarm information, and weather data.
     */
    class MainClockView final : public AbstractMainClockView {
        const std::shared_ptr<gfx::IFont> mainClockDigitFont_;       ///< Font for the main clock digits.
        const std::shared_ptr<gfx::IFont> secondClockDigitFont_;     ///< Font for the seconds in the clock.
        const std::shared_ptr<gfx::IFont> rightListFont_;            ///< Font for the right list elements (alarm & conditions).
        const std::shared_ptr<gfx::IFont> noAlarmFont_;              ///< Font for displaying "No Alarm" text.
        const std::shared_ptr<gfx::IFont> snoozeUntilFont_;          ///< Font for displaying the snooze until time.
        const std::shared_ptr<gfx::IFont> temperatureIndicatorFont_; ///< Font for the temperature indicator.

        const gfx::Pictogram pictoBell_;                              ///< Pictogram for the bell icon, used when indicating the next alarm.
        const gfx::Pictogram pictoBellFilled_;                        ///< Pictogram for the filled bell icon, used when an alarm is ringing.
        const gfx::Pictogram pictoBellSnooze_;                        ///< Pictogram for the snooze bell icon, used when an alarm is snoozed.
        const gfx::Pictogram pictoBellSlash_;                         ///< Pictogram for the bell slash icon, used when an alarm is disabled.

        const ssize_t temperatureHumiditySpacing_ {5};                ///< Spacing between temperature and humidity text.
        const ssize_t indicatorTemperatureSpacing_ {1};               ///< Spacing between temperature and indicator text.
        const ssize_t listElementBorderScreenVerticalSpacing_ {7};    ///< Vertical spacing for list elements from the screen border.
        const ssize_t listElementBorderHorizontalSpacing_ {2};        ///< Horizontal spacing for list elements from the screen border.
        const ssize_t snoozeStatusSnoozeUntilSpacing_ {3};            ///< Spacing between snooze status and snooze until text.
        const ssize_t pictogramStatusSpacing_ {2};                    ///< Spacing between the alarm status pictogram and the status text.
        const ssize_t conditionVerticalSpacing_ {4};                  ///< Vertical spacing between different conditions (indoor/outdoor).

    public:

        /**
         * @brief Constructor for the ClockView.
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
         * This method draws the current time, alarm information, and weather data on the screen.
         * @param renderer The renderer used to draw the view.
         */
        void render(RenderType& renderer) const override;

    private:

        /**
         * @brief Draws the clock on the screen.
         * This method is responsible for rendering the current time in a specific format.
         * @param renderer The renderer used to draw the clock.
         */
        void drawClock(const RenderType& renderer) const;

        /**
         * @brief Draws the alarm status on the screen.
         * This method displays the current alarm status, including whether the alarm is active,
         * snoozed, or disabled. It also handles the display of snooze until time if applicable.
         * @param renderer The renderer used to draw the alarm status.
         */
        void drawAlarmStatus(RenderType &renderer) const;

        /**
         * @brief Gets the current alarm status as a string.
         * This method checks the alarm state and returns a string representation of the current
         * alarm status, such as "No Alarm", "Alarm Active", or "Alarm Snoozed".
         * @return A string representing the current alarm status.
         */
        std::string getAlarmStatus() const;

        /**
         * @brief Gets the pictogram representing the current alarm status.
         * This method returns a pictogram based on the current alarm state, such as a bell icon
         * for an active alarm, a filled bell for a ringing alarm, or a bell slash for a disabled alarm.
         * @return A reference to the pictogram representing the current alarm status.const
         */
        const gfx::Pictogram& getAlarmStatusPictogram() const;

        /**
         * @brief Draws the conditions (temperature and humidity) on the screen.
         * This method is responsible for rendering the indoor and outdoor temperature and humidity
         * conditions at the bottom right of the screen.
         * @param renderer The renderer used to draw the conditions.
         */
        void drawConditions(const RenderType &renderer) const;

        /**
         * @brief Draws the condition (temperature, humidity, and indicator) on the screen.
         * This method is responsible for rendering the temperature, humidity, and an indicator
         * at the right of the screen.
         * The vertical position is determined by the baseline parameter.
         * @param renderer The renderer used to draw the condition.
         * @param baseline The baseline Y-coordinate for drawing the condition.
         * @param temperatureText The text representation of the temperature.
         * @param humidityText The text representation of the humidity.
         * @param indicator The text representation of the temperature indicator.
         * @return The height of the drawn condition, which can be used for further layout adjustments.
         */
        ssize_t drawSingleCondition(
            const RenderType &renderer,
            size_t baseline,
            const std::string &temperatureText,
            const std::string &humidityText,
            const std::string &indicator
        ) const;

    };

} // namespace PiAlarm::view::ssd1322

#endif //PIALARM_MAINCLOCKVIEW_H
