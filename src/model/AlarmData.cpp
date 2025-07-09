#include "model/AlarmData.hpp"

namespace PiAlarm::model {

    AlarmData::AlarmData(const Time& alarmTime, bool enabled)
        : alarmTime_{alarmTime}, alarmEnabled_{enabled}
    {}

    void AlarmData::setAlarmTime(const Time& time) {
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

    void AlarmData::setAlarmEnabled(bool enabled) {
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

    void AlarmData::setAlarm(const Time& alarm, bool enabled) {
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
