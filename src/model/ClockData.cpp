#include "model/ClockData.hpp"

namespace PiAlarm::model {

    ClockData::ClockData(const Time& current, const Time& alarm, bool enabled)
        : currentTime_{current}, alarmTime_{alarm}, alarmEnabled_{enabled}
    {}

    void ClockData::setCurrentTime(const Time& time) {
        bool shouldNotify {false};

        {
            std::lock_guard lock{mutex_};
            if (currentTime_ != time) {
                currentTime_ = time;
                shouldNotify = true;
            }
        }

        if (shouldNotify) notifyObservers();
    }

    void ClockData::setAlarmTime(const Time& time) {
        bool shouldNotify {false};

        {
            std::lock_guard lock{mutex_};
            if (alarmTime_ != time) {
                alarmTime_ = time;
                shouldNotify = true;
            }
        }

        if (shouldNotify) notifyObservers();
    }

    void ClockData::setAlarmEnabled(bool enabled) {
        bool shouldNotify {false};

        {
            std::lock_guard lock{mutex_};
            if (alarmEnabled_ != enabled) {
                alarmEnabled_ = enabled;
                shouldNotify = true;
            }
        }

        if (shouldNotify) notifyObservers();
    }

    void ClockData::setAlarm(const Time& alarm, bool enabled) {
        bool shouldNotify {false};

        {
            std::lock_guard lock{mutex_};
            if (alarmTime_ != alarm || alarmEnabled_ != enabled) {
                alarmTime_ = alarm;
                alarmEnabled_ = enabled;
                shouldNotify = true;
            }
        }

        if (shouldNotify) notifyObservers();
    }

} // namespace PiAlarm::model
