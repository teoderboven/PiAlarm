#include "model/Alarm.hpp"

namespace PiAlarm::model {

    Alarm::Alarm(Time alarmTime, bool enabled)
        : BaseModelData{}, alarmTime_{alarmTime}, alarmEnabled_{enabled}
    {}

    void Alarm::setTime(Time time) {
        bool valueChanged = setIfDifferent(alarmTime_, time);

        if (valueChanged) notifyObservers();
    }

    void Alarm::setEnabled(bool enabled) {
        bool valueChanged = setIfDifferent(alarmEnabled_, enabled);

        if (valueChanged) notifyObservers();
    }

    void Alarm::setAlarm(Time alarm, bool enabled) {
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
