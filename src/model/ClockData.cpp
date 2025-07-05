#include "model/ClockData.hpp"

namespace PiAlarm::model {

    ClockData::ClockData(const Time& current, const Time& alarm, bool enabled)
        : currentTime_{current}, alarmTime_{alarm}, alarmEnabled_{enabled}
    {}

    void ClockData::setCurrentTime(const Time& time) {
        std::lock_guard lock{mutex_};

        currentTime_ = time;
        notifyObservers();
    }

    void ClockData::setAlarmTime(const Time& time) {
        std::lock_guard lock{mutex_};

        alarmTime_ = time;
        notifyObservers();
    }

    void ClockData::setAlarmEnabled(bool enabled) {
        std::lock_guard lock{mutex_};

        alarmEnabled_ = enabled;
        notifyObservers();
    }

    void ClockData::setAlarm(const Time& alarm, bool enabled) {
        std::lock_guard lock{mutex_};

        alarmTime_ = alarm;
        alarmEnabled_ = enabled;
        notifyObservers();
    }

} // namespace PiAlarm::model
