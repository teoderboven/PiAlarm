#include "model/AlarmState.hpp"

namespace PiAlarm::model {

    void AlarmState::setTriggeredAlarm(const Alarm *alarm) {
        {
            std::lock_guard lock{mutex_};

            if (!alarm && alarm_) stop(); // If alarm is nullptr, stop the current alarm

            if (alarm_ == alarm) return; // No change, do nothing

            alarm_ = alarm;

            // Reset snooze state and set alarm ringing state
            alarmRinging_ = true;
            snoozeUntil_.reset();
            snoozeCount_ = 0;
        }

        notifyObservers();
    }

    void AlarmState::ring() {
        {
            std::lock_guard lock{mutex_};

            if (!alarm_ || alarmRinging_) return;

            alarmRinging_ = true;
            snoozeUntil_.reset();
        }

        notifyObservers();
    }

    void AlarmState::snooze(const Time& snoozeUntil) {
        {
            std::lock_guard lock{mutex_};

            if (!alarmRinging_ || snoozeUntil_.has_value()) return;

            snoozeUntil_ = snoozeUntil;
            alarmRinging_ = false;
            ++snoozeCount_;
        }

        notifyObservers();
    }

    void AlarmState::stop() {
        {
            std::lock_guard lock{mutex_};

            alarm_ = nullptr;

            alarmRinging_ = false;
            snoozeUntil_.reset();
            snoozeCount_ = 0;
        }

        notifyObservers();
    }

} // namespace PiAlarm::model
