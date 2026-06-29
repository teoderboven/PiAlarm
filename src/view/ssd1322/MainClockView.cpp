#include <algorithm>

#include "view/ssd1322/MainClockView.h"
#include "utils/ViewFormatUtils.hpp"

namespace PiAlarm::view::ssd1322 {

    MainClockView::MainClockView(
        const model::AlarmsData& alarmsData,
        const model::AlarmState& alarmStateData,
        const model::ClockData& clockData,
        const model::CurrentWeatherData& currentWeatherData,
        const model::CurrentIndoorData& temperatureSensorData,
        const model::CO2Data& co2Data
        )
        : AbstractMainClockView(
            alarmsData,
            alarmStateData,
            clockData,
            currentWeatherData,
            temperatureSensorData
        ),
        co2Data_{co2Data},

        mainClockDigitFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_Light, 48)},
        secondClockDigitFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_Light, 18)},
        rightListFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_Light, 13)},
        noAlarmFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_Light, 11)},
        snoozeUntilFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_Light, 10)},
        mainCO2AlertFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_SemiBold, 13)},
        subCO2AlertFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_SemiBold, 7)},
        temperatureIndicatorFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_Light, 7)},

        pictoBell_{"assets/pictograms/bell.png"},
        pictoBellFilled_{"assets/pictograms/bell-filled.png"},
        pictoBellSnooze_{"assets/pictograms/bell-snooze.png"},
        pictoBellSlash_{"assets/pictograms/bell-slash.png"}
    {}

    void MainClockView::render(RenderType& renderer) const {
        drawClock(renderer);
        auto alarmStatusBounds = drawAlarmStatus(renderer);
        drawCo2Alert(renderer, alarmStatusBounds);
        drawConditions(renderer);
    }

    void MainClockView::drawClock(RenderType& renderer) const {
        auto middleY = renderer.getHeight() / 2;

        auto HMDimensions = renderer.drawText(
            0, middleY,
            currentTime_.toString(false),
            mainClockDigitFont_,
            gfx::Canvas::Anchor::MiddleLeft
        );

        auto secondsY = middleY + (HMDimensions.height / 2) - 1; // -1 to align the seconds digits with the baseline of the clock digits
        renderer.drawText(
            HMDimensions.width, secondsY,
            utils::formatInt(currentTime_.second(), 2),
            secondClockDigitFont_,
            gfx::Canvas::Anchor::BottomLeft
        );
    }

    MainClockView::AlarmStatusBounds MainClockView::drawAlarmStatus(RenderType& renderer) const {
        auto rightBorder = renderer.getWidth() - listElementBorderHorizontalSpacing_;
        auto topY = listElementBorderScreenVerticalSpacing_;
        size_t snoozeOffset {0}; // width of the potential snooze until text
        auto statusFont = rightListFont_;

        std::string statusText = getAlarmStatus();

        if (!hasAlarmEnabled_)
            statusFont = noAlarmFont_; // text overlaps with clock digits with rightListFont

        if (alarmStateData_.isAlarmSnoozed()) {
            // first draw snooze until time if snooze is active

            auto snoozeUntilDimensions = renderer.drawText(
                rightBorder, topY + (3/2), // 3/2 to center the snooze until text vertically
                '(' + alarmStateData_.getSnoozeUntil()->toString() + ')',
                snoozeUntilFont_,
                gfx::Canvas::Anchor::TopRight
            );
            snoozeOffset = snoozeUntilDimensions.width + snoozeStatusSnoozeUntilSpacing_;
        }

        // draw the alarm status text
        auto statusTextDimensions = renderer.drawText(
            rightBorder - snoozeOffset, topY,
            statusText,
            statusFont,
            gfx::Canvas::Anchor::TopRight
        );

        // draw the pictogram representing the status
        auto pictogram = getAlarmStatusPictogram();
        auto pictogramX = rightBorder - (snoozeOffset + statusTextDimensions.width + pictogram.getWidth() + pictogramStatusSpacing_);
        auto pictogramY = topY + (statusTextDimensions.height / 2) - (pictogram.getHeight() / 2) + 1; // +2 to align bottom of the clock with the baseline

        auto savedDrawMode = renderer.getDrawMode();
        renderer.setDrawMode(gfx::Canvas::DrawMode::Invert); // Pictograms files are black on white background

        renderer.drawPictogram(pictogramX, pictogramY, pictogram);

        renderer.setDrawMode(savedDrawMode);

        return {pictogramX, pictogramY + pictogram.getHeight()}; // return the bounding box of the alarm status area
    }

    std::string MainClockView::getAlarmStatus() const {
        if (!hasAlarmEnabled_)
            return "Pas d'alarme";

        if (!alarmStateData_.hasTriggeredAlarm())
            return  nextAlarmTime_.toString(false);

        if (alarmStateData_.isAlarmRinging())
            return "DRIIIING !";

        if (alarmStateData_.isAlarmSnoozed())
            return "Snooze";

        return "???";
    }

    const gfx::Pictogram& MainClockView::getAlarmStatusPictogram() const {
        if (!hasAlarmEnabled_)
            return pictoBellSlash_;

        if (alarmStateData_.isAlarmRinging())
            return pictoBellFilled_;

        if (alarmStateData_.isAlarmSnoozed())
            return pictoBellSnooze_;

        return pictoBell_;
    }

    void MainClockView::drawCo2Alert(RenderType& renderer, const AlarmStatusBounds& bounds) const {
        if (!isAlertLevel(co2Data_.getAirQualityLevel()) || !co2Data_.isValid()) return;

        if (co2Data_.getAirQualityLevel() == model::AirQualityLevel::VeryPoor
            && currentTime_.second() % 3 == 2)
                return; // blink the alert every 3 seconds (2 on, 1 off)

        auto rightBorder = bounds.leftX - co2AlertStatusSpacing_;
        auto topY = listElementBorderScreenVerticalSpacing_;

        auto subTextDimensions = renderer.drawText(
            rightBorder, topY + mainCO2AlertFont_->getAscender(),
            "2",
            subCO2AlertFont_,
            gfx::Canvas::Anchor::BottomRight
        );

        renderer.drawText(
            rightBorder - subTextDimensions.width, topY,
            "CO",
            mainCO2AlertFont_,
            gfx::Canvas::Anchor::TopRight
        );
    }

    bool MainClockView::isAlertLevel(model::AirQualityLevel level) {
        using model::AirQualityLevel;
        return level == AirQualityLevel::Poor || level == AirQualityLevel::VeryPoor;
    }

    void MainClockView::drawConditions(RenderType& renderer) const {
        auto bottomY = renderer.getHeight() - listElementBorderScreenVerticalSpacing_;

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
            bottomY - (outdoorTextHeight + conditionVerticalSpacing_),
            utils::formatTemperature(currentIndoorTemperature_, indoorDataValid_),
            utils::formatHumidity(currentIndoorHumidity_, indoorDataValid_),
            "Int."
        );
    }

    ssize_t MainClockView::drawSingleCondition(
        RenderType& renderer,
        size_t baseline,
        const std::string &temperatureText,
        const std::string &humidityText,
        const std::string &indicator) const
    {
        const auto rightBorder = renderer.getWidth() - listElementBorderHorizontalSpacing_;

        // draw humidity at bottom right of the screen
        const auto humiditySize = renderer.drawText(
            rightBorder,
            baseline,
            humidityText,
            rightListFont_,
            gfx::Canvas::Anchor::BottomRight
        );

        // draw temperature at the left of the humidity
        const auto temperatureX = rightBorder - humiditySize.width - temperatureHumiditySpacing_;
        const auto temperatureSize = renderer.drawText(
            temperatureX,
            baseline,
            temperatureText,
            rightListFont_,
            gfx::Canvas::Anchor::BottomRight
        );

        // draw temperature indicator at the bottom left of the temperature
        const auto indicatorX = rightBorder - (temperatureSize.width + humiditySize.width
                                         + temperatureHumiditySpacing_ + indicatorTemperatureSpacing_);
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
