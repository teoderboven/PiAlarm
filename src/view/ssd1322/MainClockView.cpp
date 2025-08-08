#include "view/ssd1322/MainClockView.h"
#include "utils/ViewUtils.hpp"

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
        secondClockDigitFont_{gfx::TrueTypeFontCache::getFont("assets/fonts/MozillaText-Regular.ttf", 18)}
    {}

    void MainClockView::render(RenderType &renderer) const {
        drawClock(renderer);
    }

    void MainClockView::drawClock(const RenderType &renderer) const {
        auto middleY = renderer.getHeight() / 2;
        auto dimensions = renderer.drawText(
            0, middleY,
            currentTime_.toString(false),
            mainClockDigitFont_,
            gfx::Canvas::Anchor::MiddleLeft
        );
        renderer.drawText(
            dimensions.width, (middleY + dimensions.height / 2),
            utils::formatInt(currentTime_.second(), 2),
            secondClockDigitFont_,
            gfx::Canvas::Anchor::BottomLeft
        );
    }

} // namespace PiAlarm::view::ssd1322
