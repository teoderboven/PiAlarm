#ifndef PIALARM_MAINCLOCKVIEW_H
#define PIALARM_MAINCLOCKVIEW_H

#include "view/AbstractMainClockView.h"
#include "gfx/TrueTypeFont.h"
#include "gfx/TrueTypeFontCache.h"

namespace PiAlarm::view::ssd1322 {

    /**
     * @class MainClockView
     * @brief Concrete implementation of the main clock view for the SSD1322 display.
     *
     * This class extends AbstractMainClockView and provides the rendering logic for the main clock view,
     * including displaying the current time, alarm information, and weather data.
     */
    class MainClockView final : public AbstractMainClockView {
        std::shared_ptr<gfx::IFont> mainClockDigitFont_;       ///< Font for the main clock digits.
        std::shared_ptr<gfx::IFont> secondClockDigitFont_;     ///< Font for the seconds in the clock.
        std::shared_ptr<gfx::IFont> rightListFont_;            ///< Font for the right list elements (alarm & conditions).
        std::shared_ptr<gfx::IFont> temperatureIndicatorFont_; ///< Font for the temperature indicator.

        ssize_t temperatureHumiditySpacing {5};                ///< Spacing between temperature and humidity text.
        ssize_t indicatorTemperatureSpacing {1};               ///< Spacing between temperature and indicator text.
        ssize_t listElementBorderScreenVerticalSpacing {7};    ///< Vertical spacing for list elements from the screen border.
        ssize_t conditionVerticalSpacing {4};                  ///< Vertical spacing between different conditions (indoor/outdoor).

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
