#include <stdexcept>
#include <cassert>

#include "model/manager/AlarmManager.h"

namespace PiAlarm::model::manager {

    AlarmManager::AlarmManager(
        const ClockData& clockData,
        const AlarmsData& alarmsData,
        std::chrono::minutes snoozeDuration,
        std::chrono::minutes ringDuration
    )
        : clockData_{clockData},
        alarmsData_{alarmsData},
        snoozeDuration_{snoozeDuration},
        ringDuration_{ringDuration}
    {
        if (snoozeDuration.count() <= 0) {
            throw std::invalid_argument("Snooze duration must be greater than zero.");
        }
        if (ringDuration.count() <= 0) {
            throw std::invalid_argument("Ring duration must be greater than zero.");
        }

        clockData_.addObserver(this);
    }

    AlarmManager::~AlarmManager() {
        clockData_.removeObserver(this);
    }

    void AlarmManager::snoozeAlarm() {
        auto snoozeUntil {clockData_.getCurrentTime() + snoozeDuration_};

        state_.snooze(snoozeUntil);
    }

    void AlarmManager::stopAlarm() {
        if (state_.hasTriggeredAlarm()) {
            lastStoppedAlarm_ = state_.getTriggeredAlarm();
            lastStoppedAlarmTime_ = lastStoppedAlarm_->getTime();
            lastStopTime_ = clockData_.getCurrentTime();
        }

        state_.stop();
    }

    void AlarmManager::update() {
        checkAndResetLastStoppedAlarm();

        if (!state_.hasTriggeredAlarm()) detectTriggeredAlarm(); // may set a triggered alarm

        if (state_.hasTriggeredAlarm()) processTriggeredAlarm();
    }

    void AlarmManager::checkAndResetLastStoppedAlarm() {
        if (!lastStoppedAlarm_) return;

        const auto& currentTime = clockData_.getCurrentTime();
        auto elapsed = currentTime.secondsSince(lastStopTime_);

        // if the elapsed time since the last stop exceeds the ring duration,
        // or if the last stopped alarm's time has changed, reset the last stopped alarm.
        if (elapsed >= ringDuration_ || lastStoppedAlarm_->getTime() != lastStoppedAlarmTime_) {
            lastStoppedAlarm_ = nullptr;
        }
    }

    void AlarmManager::detectTriggeredAlarm() {
        const auto& currentTime {clockData_.getCurrentTime()};
        const Alarm* mostRecentAlarm {nullptr};
        std::chrono::seconds minDiff {std::chrono::seconds::max()};

        for (const auto& alarm : alarmsData_) {
            if (!alarm.isEnabled()) continue;

            if (isAlarmInhibited(alarm, currentTime)) continue;

            auto diff {currentTime.secondsSince(alarm.getTime())};

            if (diff < minDiff) {
                minDiff = diff;
                mostRecentAlarm = &alarm;
            }
        }

        if (mostRecentAlarm && minDiff <= ringDuration_) {
            state_.setTriggeredAlarm(mostRecentAlarm);
        }
    }

    void AlarmManager::processTriggeredAlarm() {
        const auto& currentTime {clockData_.getCurrentTime()};
        const auto* triggeredAlarm {state_.getTriggeredAlarm()};

        assert(triggeredAlarm); // Ensure that there is a triggered alarm

        if (!isInAlarmWindow(*triggeredAlarm, currentTime)) {
            stopAlarm();
            return;
        }

        if (shouldRingAfterSnooze(currentTime)) {
            state_.ring();
        }
    }

    bool AlarmManager::isAlarmInhibited(const Alarm& alarm, Time currentTime) const {
        if (!lastStoppedAlarm_) return false;

        if (&alarm != lastStoppedAlarm_) return false; // alarms are stored in AlarmsData and their addresses are stable

        if (alarm.getTime() != lastStoppedAlarmTime_) return false; // the alarm time has changed

        auto elapsed = currentTime.secondsSince(lastStopTime_);

        return elapsed < ringDuration_;
    }

    bool AlarmManager::isInAlarmWindow(const Alarm& alarm, const Time& currentTime) const {
        const auto alarmEndTime = getAlarmRingWindowEnd(alarm);

        return currentTime.isBetween(alarm.getTime(), alarmEndTime);
    }

    bool AlarmManager::shouldRingAfterSnooze(const Time& currentTime) const {
        if (!state_.isAlarmSnoozed()) return false;

        const auto snoozeUntilOpt = state_.getSnoozeUntil();
        if (!snoozeUntilOpt.has_value()) return false;

        const auto triggeredAlarm = state_.getTriggeredAlarm();
        if (!triggeredAlarm) return false; // Ensure there is a triggered alarm

        const auto snoozeUntil = snoozeUntilOpt.value();
        const auto alarmRingWindowEnd = getAlarmRingWindowEnd(*triggeredAlarm);

        return currentTime.isBetween(snoozeUntil, alarmRingWindowEnd);
    }

    Time AlarmManager::getAlarmRingWindowEnd(const Alarm& alarm) const {
        return alarm.getTime() + ringDuration_ + (state_.getSnoozeCount() * snoozeDuration_);
    }

} // namespace PiAlarm::model::manager
