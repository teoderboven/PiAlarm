#include "model/AlarmData.hpp"

namespace PiAlarm::model {

    AlarmData::AlarmData(const Time& alarmTime, bool enabled)
        : BaseModelData{}, alarmTime_{alarmTime}, alarmEnabled_{enabled}
    {}

    void AlarmData::setAlarmTime(const Time& time) {
        bool valueChanged = setIfDifferent(alarmTime_, time);

        if (valueChanged) notifyObservers();
    }

    void AlarmData::setAlarmEnabled(bool enabled) {
        bool valueChanged = setIfDifferent(alarmEnabled_, enabled);

        if (valueChanged) notifyObservers();
    }

    void AlarmData::setAlarm(const Time& alarm, bool enabled) {
        bool valueChanged {false};

        {
            std::lock_guard lock{mutex_};
            if (alarmTime_ != alarm || alarmEnabled_ != enabled) {
                alarmTime_ = alarm;
                alarmEnabled_ = enabled;
                valueChanged = true;
            }
        }

        if (valueChanged) notifyObservers();
    }

} // namespace PiAlarm::model
