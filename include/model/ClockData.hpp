#ifndef CLOCKDATA_HPP
#define CLOCKDATA_HPP

#include "model/Time.h"
#include "common/Observable.hpp"

/**
 * @namespace PiAlarm::model
 * @brief Contains the data models for the PiAlarm application.
 */
namespace PiAlarm::model {

    /**
     * @class ClockData
     * @brief Represents the data model for a clock, including current time, alarm time, and alarm status.
     *
     * This class extends the Observable class to notify observers of changes in the clock data.
     */
    class ClockData : public common::Observable{
        Time currentTime_;
        Time alarmTime_;
        bool alarmEnabled_ = false;

    public:
        /**
         * Default constructor for ClockData.
         */
        ClockData() = default;

        /**
         * Constructs a ClockData object with specified current time, alarm time, and alarm status.
         * @param current The current time to set.
         * @param alarm The alarm time to set.
         * @param enabled True if the alarm is enabled, false otherwise.
         */
        ClockData(const Time& current, const Time& alarm, bool enabled)
        : currentTime_{current}, alarmTime_{alarm}, alarmEnabled_{enabled}
        {}

        /**
         * Sets the current time and notifies observers of the change.
         * @param time The new current time to set.
         */
        void setCurrentTime(const Time& time) {
            currentTime_ = time;
            notifyObservers();
        }

        /**
         * Sets the alarm time and notifies observers of the change.
         * @param time The new alarm time to set.
         */
        void setAlarmTime(const Time& time) {
            alarmTime_ = time;
            notifyObservers();
        }

        /**
         * Enables or disables the alarm and notifies observers of the change.
         * @param enabled True to enable the alarm, false to disable it.
         */
        void setAlarmEnabled(bool enabled) {
            alarmEnabled_ = enabled;
            notifyObservers();
        }

        /**
         * Sets both the alarm time and its enabled status, notifying observers of the change.
         * @param alarm The new alarm time to set.
         * @param enabled True to enable the alarm, false to disable it.
         */
        void setAlarm(const Time& alarm, bool enabled) {
            alarmTime_ = alarm;
            alarmEnabled_ = enabled;
            notifyObservers();
        }

        /**
         * Gets the current time.
         * @return A reference to the current time.
         */
        [[nodiscard]]
        const Time& getCurrentTime() const {
            return currentTime_;
        }

        /**
         * Gets the alarm time.
         * @return A reference to the alarm time.
         */
        [[nodiscard]]
        const Time& getAlarmTime() const {
            return alarmTime_;
        }

        /**
         * Checks if the alarm is enabled.
         * @return A reference to the alarm enabled status.
         */
        [[nodiscard]]
        bool isAlarmEnabled() const {
            return alarmEnabled_;
        }

    };

} // namespace PiAlarm::model

#endif //CLOCKDATA_HPP
