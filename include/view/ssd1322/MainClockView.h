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
        std::shared_ptr<gfx::IFont> mainClockDigitFont_;
        std::shared_ptr<gfx::IFont> secondClockDigitFont_;

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
        void render(RenderType& renderer) override;

    private:

        /**
         * @brief Draws the clock on the screen.
         * This method is responsible for rendering the current time in a specific format.
         * @param renderer The renderer used to draw the clock.
         */
        void drawClock(const RenderType& renderer);

    };

} // namespace PiAlarm::view::ssd1322

#endif //PIALARM_MAINCLOCKVIEW_H
