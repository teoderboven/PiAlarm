#include <algorithm>

#include "view/ssd1322/MainClockView.h"
#include "utils/ViewFormatUtils.hpp"

namespace PiAlarm::view::ssd1322 {

    MainClockView::MainClockView(
        const model::AlarmsData& alarmsData,
        const model::AlarmState& alarmStateData,
        const model::ClockData &clockData,
        const model::CurrentWeatherData &currentWeatherData,
        const model::TemperatureSensorData& temperatureSensorData
        )
        : AbstractMainClockView(
            alarmsData,
            alarmStateData,
            clockData,
            currentWeatherData,
            temperatureSensorData
        ),
        mainClockDigitFont_{gfx::TrueTypeFontCache::getFont("assets/fonts/MozillaText-Regular.ttf", 48)},
        secondClockDigitFont_{gfx::TrueTypeFontCache::getFont("assets/fonts/MozillaText-Regular.ttf", 18)},
        rightListFont_{gfx::TrueTypeFontCache::getFont("assets/fonts/MozillaText-Light.ttf", 13)},
        temperatureIndicatorFont_{gfx::TrueTypeFontCache::getFont("assets/fonts/MozillaText-Light.ttf", 7)}
    {}

    void MainClockView::render(RenderType &renderer) const {
        drawClock(renderer);
        drawConditions(renderer);
    }

    void MainClockView::drawClock(const RenderType &renderer) const {
        auto middleY = renderer.getHeight() / 2;

        auto HMDimensions = renderer.drawText(
            0, middleY,
            currentTime_.toString(false),
            mainClockDigitFont_,
            gfx::Canvas::Anchor::MiddleLeft
        );

        renderer.drawText(
            HMDimensions.width, (middleY + HMDimensions.height / 2),
            utils::formatInt(currentTime_.second(), 2),
            secondClockDigitFont_,
            gfx::Canvas::Anchor::BottomLeft
        );
    }

    void MainClockView::drawConditions(const RenderType &renderer) const {
        auto bottomY = renderer.getHeight() - listElementBorderScreenVerticalSpacing;

        // draw outdoor condition
        auto outdoorTextHeight = drawSingleCondition(
            renderer,
            bottomY,
            utils::formatTemperature(currentOutdoorTemperature_, currentWeatherDataValid_),
            utils::formatHumidity(currentOutdoorHumidity_, currentWeatherDataValid_),
            "Ext."
        );

        // draw indoor condition above outdoor condition
        drawSingleCondition(
            renderer,
            bottomY - (outdoorTextHeight + conditionVerticalSpacing),
            utils::formatTemperature(currentIndoorTemperature_, sensorDataValid_),
            utils::formatHumidity(currentIndoorHumidity_, sensorDataValid_),
            "Int."
        );
    }

    ssize_t MainClockView::drawSingleCondition(
        const RenderType &renderer,
        size_t baseline,
        const std::string &temperatureText,
        const std::string &humidityText,
        const std::string &indicator) const
    {
        const auto rightBorder = renderer.getWidth();

        // draw humidity at bottom right of the screen
        const auto humiditySize = renderer.drawText(
            rightBorder,
            baseline,
            humidityText,
            rightListFont_,
            gfx::Canvas::Anchor::BottomRight
        );

        // draw temperature at the left of the humidity
        const auto temperatureX = rightBorder - humiditySize.width - temperatureHumiditySpacing;
        const auto temperatureSize = renderer.drawText(
            temperatureX,
            baseline,
            temperatureText,
            rightListFont_,
            gfx::Canvas::Anchor::BottomRight
        );

        // draw temperature indicator at the bottom left of the temperature
        const auto indicatorX = rightBorder - (temperatureSize.width + humiditySize.width
                                         + temperatureHumiditySpacing + indicatorTemperatureSpacing);
        renderer.drawText(
            indicatorX,
            baseline,
            indicator,
            temperatureIndicatorFont_,
            gfx::Canvas::Anchor::BottomRight
        );

        return std::max(temperatureSize.height, humiditySize.height);
    }

} // namespace PiAlarm::view::ssd1322
