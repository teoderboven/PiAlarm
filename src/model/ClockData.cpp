#include "model/ClockData.hpp"

namespace PiAlarm::model {

    ClockData::ClockData(const Time& current, const Time& alarm, bool enabled)
        : currentTime_{current}, alarmTime_{alarm}, alarmEnabled_{enabled}
    {}

    void ClockData::setCurrentTime(const Time& time) {
        currentTime_ = time;
        notifyObservers();
    }

    void ClockData::setAlarmTime(const Time& time) {
        alarmTime_ = time;
        notifyObservers();
    }

    void ClockData::setAlarmEnabled(bool enabled) {
        alarmEnabled_ = enabled;
        notifyObservers();
    }

    void ClockData::setAlarm(const Time& alarm, bool enabled) {
        alarmTime_ = alarm;
        alarmEnabled_ = enabled;
        notifyObservers();
    }

} // namespace PiAlarm::model
