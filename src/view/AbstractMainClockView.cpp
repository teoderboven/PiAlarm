#include "view/AbstractMainClockView.h"

namespace PiAlarm::view {
    
    AbstractMainClockView::AbstractMainClockView(
        const model::AlarmsData& alarmsData,
        const model::AlarmState& alarmStateData,
        const model::ClockData &clockData,
        const model::CurrentWeatherData &currentWeatherData,
        const model::CurrentIndoorData& temperatureSensorData
        )
        : AbstractObserverView{true},
        alarmsData_{alarmsData},
        alarmStateData_{alarmStateData},
        clockData_{clockData},
        currentWeatherData_{currentWeatherData},
        currentIndoorData_{temperatureSensorData}
    {
        alarmsData_.addObserver(this);
        alarmStateData_.addObserver(this);
        clockData_.addObserver(this);
        currentWeatherData_.addObserver(this);
        currentIndoorData_.addObserver(this);
    }

    AbstractMainClockView::~AbstractMainClockView() {
        alarmsData_.removeObserver(this);
        alarmStateData_.removeObserver(this);
        clockData_.removeObserver(this);
        currentWeatherData_.removeObserver(this);
        currentIndoorData_.removeObserver(this);
    }

    void AbstractMainClockView::refresh() {
        currentTime_ = clockData_.getCurrentTime();

        enabledAlarmCount_ = alarmsData_.enabledAlarmCount();
        hasAlarmEnabled_ = enabledAlarmCount_ > 0;
        const auto nextAlarm {alarmsData_.getNextAlarm(currentTime_)};
        nextAlarmTime_ = nextAlarm ? nextAlarm->getTime() : model::Time(0);

        currentIndoorTemperature_ = currentIndoorData_.getTemperature();
        currentIndoorHumidity_ = currentIndoorData_.getHumidity();
        indoorDataValid_ = currentIndoorData_.isValid();

        currentOutdoorTemperature_ = currentWeatherData_.getTemperature();
        currentOutdoorHumidity_ = currentWeatherData_.getHumidity();
        currentOutdoorPressure_ = currentWeatherData_.getPressure();
        currentWeatherCondition_ = currentWeatherData_.getCondition();
        currentWeatherDataValid_ = currentWeatherData_.isValid();
    }
    
} // namespace PiAlarm::view
