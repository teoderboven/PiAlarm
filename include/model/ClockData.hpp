#ifndef CLOCKDATA_HPP
#define CLOCKDATA_HPP

#include <mutex>

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
    class ClockData final : public common::Observable{
        Time currentTime_;
        Time alarmTime_;
        bool alarmEnabled_ = false;

        // mutable to allow usage of mutex_ in const methods
        mutable std::mutex mutex_; ///< Protects access to data (multithreading)

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
        ClockData(const Time& current, const Time& alarm, bool enabled);

        /**
         * Sets the current time and notifies observers of the change.
         * @param time The new current time to set.
         */
        void setCurrentTime(const Time& time);

        /**
         * Sets the alarm time and notifies observers of the change.
         * @param time The new alarm time to set.
         */
        void setAlarmTime(const Time& time);

        /**
         * Enables or disables the alarm and notifies observers of the change.
         * @param enabled True to enable the alarm, false to disable it.
         */
        void setAlarmEnabled(bool enabled);

        /**
         * Sets both the alarm time and its enabled status, notifying observers of the change.
         * @param alarm The new alarm time to set.
         * @param enabled True to enable the alarm, false to disable it.
         */
        void setAlarm(const Time& alarm, bool enabled);

        /**
         * Gets the current time.
         * @return A reference to the current time.
         */
        [[nodiscard]]
        inline const Time& getCurrentTime() const;

        /**
         * Gets the alarm time.
         * @return A reference to the alarm time.
         */
        [[nodiscard]]
        inline const Time& getAlarmTime() const;

        /**
         * Checks if the alarm is enabled.
         * @return A reference to the alarm enabled status.
         */
        [[nodiscard]]
        inline bool isAlarmEnabled() const;

    };

    // inline methods implementations

    inline const Time& ClockData::getCurrentTime() const {
        std::lock_guard lock{mutex_};

        return currentTime_;
    }

    inline const Time& ClockData::getAlarmTime() const {
        std::lock_guard lock{mutex_};

        return alarmTime_;
    }

    inline bool ClockData::isAlarmEnabled() const {
        std::lock_guard lock{mutex_};

        return alarmEnabled_;
    }

} // namespace PiAlarm::model

#endif //CLOCKDATA_HPP
