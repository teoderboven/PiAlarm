#ifndef ALARMMANAGER_H
#define ALARMMANAGER_H

#include <chrono>
#include <optional>

#include "common/Observer.h"
#include "model/ClockData.hpp"
#include "model/Alarm.hpp"
#include "model/AlarmsData.hpp"
#include "model/AlarmState.hpp"

/**
 * @namespace PiAlarm::model::manager
 * @brief
 */
namespace PiAlarm::model::manager {

    /**
     * @class AlarmManager
     * @brief Manages alarm functionalities in the PiAlarm application.
     *
     * This class is responsible for handling alarm operations such as snoozing,
     * stopping, and updating the alarm state based on the current time and configured alarms.
     * It observes the clock data to trigger alarms at the appropriate times.
     */
    class AlarmManager final : public common::Observer {
        const ClockData& clockData_; ///< Reference to the clock data model.
        const AlarmsData& alarmsData_; ///< Reference to the alarms data model.
        AlarmState state_; ///< The state data for the current alarm.

        const std::chrono::minutes snoozeDuration_; ///< Duration for which the alarm can be snoozed.
        const std::chrono::minutes ringDuration_; ///< Effective duration for which the alarm rings when triggered

    public:

        /**
         * @brief Constructs an AlarmManager with the provided clock and alarms data.
         * @param clockData Reference to the clock data model.
         * @param alarmsData Reference to the alarms data model.
         * @param snoozeDuration The duration for which the alarm can be snoozed.
         * @param ringDuration The effective duration for which the alarm rings when triggered.
         * @throws std::invalid_argument if snoozeDuration is zero.
         * @throws std::invalid_argument if ringDuration is zero.
         */
        explicit AlarmManager(
            const ClockData& clockData,
            const AlarmsData& alarmsData,
            std::chrono::minutes snoozeDuration = std::chrono::minutes(5),
            std::chrono::minutes ringDuration = std::chrono::minutes(60)
        );

        /**
         * @brief Destructor for AlarmManager.
         * Cleans up resources and stops observing the clock data.
         */
        ~AlarmManager() override;

        /**
         * @brief Snoozes the alarm if it is currently ringing.
         * @note The duration for snoozing is set to the snoozeDuration specified during construction.
         */
        void snoozeAlarm();

        /**
         * @brief Stops the alarm if it is currently active.
         */
        void stopAlarm();

        /**
         * @brief Returns the current alarm state.
         * This method provides access to the current alarm state, which includes information about
         * the currently triggered alarm, whether it is ringing, and snooze settings.
         * @return A const reference to the AlarmState object representing the current alarm state.
         */
        inline const AlarmState& getAlarmState() const;

        /**
         * @brief Updates the alarm state based on the current time and alarm settings.
         * This method checks if the current time matches the alarm time and triggers the alarm if necessary.
         */
        void update() override;

    private:

        /**
         * @brief Detects if there is a currently triggered alarm.
         *
         * This method searches through the enabled alarms and checks if any of them should be ringing
         * based on the current time and the configured ring duration. If a triggered alarm is found,
         * it updates the state to reflect this.
         *
         * @note This method can calls state_.setTriggeredAlarm() that causes the alarm to immediately enter the ringing state.
         *
         * @note This method updates the state_ with the most recent alarm that is currently ringing if any.
         */
        void detectTriggeredAlarm();

        /**
         * @brief Processes the currently triggered alarm based on the current time.
         *
         * This method checks whether the currently triggered alarm is still within its valid
         * ringing window (including snoozed duration). If the alarm is outside this window,
         * it will be stopped. If the alarm is snoozed and the snooze period has expired,
         * the alarm will resume ringing.
         *
         * @pre An alarm must have been triggered previously (state_.hasTriggeredAlarm() == true).
         *
         * @note May stop the alarm if it has expired.
         * @note May cause the alarm to start ringing again if snooze period has ended.
         *
         * @see detectTriggeredAlarm()
         */
        void processTriggeredAlarm();

        /**
         * @brief Checks if the current time is within the alarm's active window.
         * This method determines if the current time falls within the alarm's active window,
         * which is defined by the alarm's time and its ring duration.
         * @param alarm The alarm to check against.
         * @param currentTime The current time to compare with the alarm time.
         * @return True if the current time is within the alarm's active window, false otherwise.
         */
        [[nodiscard]]
        bool isInAlarmWindow(const Alarm& alarm, const Time& currentTime) const;

        /**
         * @brief Determines if the alarm should ring after being snoozed.
         * This method checks if the current time is past the snooze time and if the alarm should ring again.
         * @param currentTime The current time to check against.
         * @return True if the alarm should ring after snooze, false otherwise.
         */
        [[nodiscard]]
        bool shouldRingAfterSnooze(const Time& currentTime) const;

        /**
         * @brief Calculates the end time of the ringing window for a given alarm.
         *
         * The ringing window represents the total duration during which the alarm
         * is allowed to ring, including any snoozed periods. The end of this window
         * is defined as:
         *
         * `alarm time + ring duration + (snooze count × snooze duration)`
         *
         * This value is used to determine whether the alarm should still be active
         * or should be stopped.
         *
         * @param alarm The alarm for which to calculate the ringing window end time.
         * @return The time at which the ringing window for the given alarm ends.
         *
         * @pre The snooze count is assumed to be accurate for the given alarm.
         */
        [[nodiscard]]
        Time getAlarmRingWindowEnd(const Alarm& alarm) const;

    };

    // Inline method implementation

    inline const AlarmState& AlarmManager::getAlarmState() const {
        return state_;
    }

} // namespace PiAlarm::model::manager

#endif //ALARMMANAGER_H
