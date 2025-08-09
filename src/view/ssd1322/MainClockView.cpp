#include <algorithm>

#include "view/ssd1322/MainClockView.h"
#include "utils/ViewFormatUtils.hpp"

// Fonts used in the view
#define FONT_MozillaText_Regular "assets/fonts/MozillaText-Regular.ttf"
#define FONT_MozillaText_Light "assets/fonts/MozillaText-Light.ttf"

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
        mainClockDigitFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_Light, 48)},
        secondClockDigitFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_Light, 18)},
        rightListFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_Light, 13)},
        noAlarmFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_Light, 12)},
        snoozeUntilFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_Light, 10)},
        temperatureIndicatorFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_Light, 7)},

        pictoBell_{"assets/pictograms/bell.png"},
        pictoBellFilled_{"assets/pictograms/bell-filled.png"},
        pictoBellSnooze_{"assets/pictograms/bell-snooze.png"},
        pictoBellSlash_{"assets/pictograms/bell-slash.png"}
    {}

    void MainClockView::render(RenderType &renderer) const {
        drawClock(renderer);
        drawAlarmStatus(renderer);
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

    void MainClockView::drawAlarmStatus(RenderType &renderer) const {
        auto rightBorder = renderer.getWidth();
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
    }

    std::string MainClockView::getAlarmStatus() const {
        if (!hasAlarmEnabled_)
            return "Pas d'alarme active";

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

    void MainClockView::drawConditions(const RenderType &renderer) const {
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
