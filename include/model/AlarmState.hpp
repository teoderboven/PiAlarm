#ifndef ALARMSTATE_HPP
#define ALARMSTATE_HPP

#include <mutex>
#include <optional>

#include "common/Observable.hpp"
#include "model/Alarm.hpp"

namespace PiAlarm::model {

    /**
     * @class AlarmState
     * @brief Represents the state of the currently ringing alarm in the PiAlarm application.
     *
     * This class extends BaseModelData and implements common::Observable to notify observers
     * when the currently ringing alarm changes.
     */
    class AlarmState final : public common::Observable {
        const Alarm* alarm_ = nullptr; ///< Pointer to the current triggered alarm
        bool alarmRinging_ = false; ///< Flag indicating if the alarm is currently ringing
        std::optional<Time> snoozeUntil_; ///< Time until which the alarm is snoozed, if applicable
        int snoozeCount_ = 0; ///< Counter for the number of times the alarm has been snoozed

        mutable std::mutex mutex_; ///< Mutex for thread-safe access to the alarm state data

    public:

        /**
         * @brief Default constructor for AlarmState.
         * Initializes the alarm state data with no active alarm.
         */
        AlarmState() = default;

        /**
         * @brief Sets the currently triggered alarm.
         * This method updates the currently triggered alarm and resets the snooze count and states.
         * @param alarm Pointer to the Alarm that is currently triggered, or nullptr if no alarm is active.
         */
        void setTriggeredAlarm(const Alarm* alarm);

        /**
         * @brief Rings the currently triggered alarm.
         * This method sets the alarm as ringing and resets the snooze state.
         * @note This method should be called only when an alarm is set.
         *       If no alarm is set, it will not have any effect.
         */
        void ring();

        /**
         * @brief Snoozes the currently ringing alarm until the specified time.
         * This method sets the snooze state for the currently ringing alarm and increments the snooze count.
         * @note This method should be called only when an alarm is currently ringing.
         *       If no alarm is ringing, it will not have any effect.
         * @param snoozeUntil The time until which the alarm should be snoozed.
         */
        void snooze(const Time& snoozeUntil);

        /**
         * @brief Stops the currently ringing alarm.
         * This method clears the currently triggered alarm and resets the snooze state.
         * @note This method should be called to stop the alarm when it is no longer needed.
         */
        void stop();

        /**
         * @brief Retrieves the currently ringing alarm.
         * This method returns a pointer to the current ringing alarm, or nullptr if no alarm is ringing.
         * @warning Even if you checked if there is a ringing alarm using hasRingingAlarm(),
         *          always verify the returned pointer is not nullptr before dereferencing it.
         * @return Pointer to the currently ringing Alarm, or nullptr if no alarm is active.
         */
        inline const Alarm* getTriggeredAlarm() const;

        /**
         * @brief Checks if there is a currently ringing alarm.
         * This method returns true if the current ringing alarm is not nullptr, indicating that an alarm is active.
         * @return True if there is a ringing alarm, false otherwise.
         */
        inline bool hasTriggeredAlarm() const;

        /**
         * @brief Checks if the alarm is currently ringing.
         * This method returns true if the alarm is currently ringing.
         * @note If there is no currently triggered alarm, will always return false.
         * @return True if the alarm is ringing, false otherwise.
         */
        inline bool isAlarmRinging() const;

        /**
         * @brief Checks if the alarm is currently snoozed.
         * This method returns true if the alarm is currently snoozed.
         * @note If there is no currently triggered alarm, will always return false.
         * @return True if the alarm is snoozed, false otherwise.
         */
        inline bool isAlarmSnoozed() const;

        /**
         * @brief Gets the time until which the alarm is snoozed.
         * This method returns the time until which the alarm is currently snoozed, if applicable.
         * @return An optional containing the snooze time if the alarm is snoozed, or an empty optional if not.
         */
        inline std::optional<Time> getSnoozeUntil() const;

        /**
         * @brief Gets the number of times the alarm has been snoozed.
         * This method returns the count of how many times the alarm has been snoozed.
         * This count is incremented each time the snooze method is called.
         * @note If there is no currently triggered alarm, this will return 0.
         * @return The number of snoozes.
         */
        inline int getSnoozeCount() const;

    };

    // Inline method implementations

    inline const Alarm* AlarmState::getTriggeredAlarm() const {
        std::lock_guard lock{mutex_};

        return alarm_;
    }

    inline bool AlarmState::hasTriggeredAlarm() const {
        std::lock_guard lock{mutex_};

        return alarm_ != nullptr;
    }

    inline bool AlarmState::isAlarmRinging() const {
        std::lock_guard lock{mutex_};

        return alarmRinging_;
    }

    inline bool AlarmState::isAlarmSnoozed() const {
        std::lock_guard lock{mutex_};

        return snoozeUntil_.has_value();
    }

    inline std::optional<Time> AlarmState::getSnoozeUntil() const {
        std::lock_guard lock{mutex_};

        return snoozeUntil_;
    }

    inline int AlarmState::getSnoozeCount() const {
        std::lock_guard lock{mutex_};

        return snoozeCount_;
    }

} // namespace PiAlarm::model

#endif //ALARMSTATE_HPP
