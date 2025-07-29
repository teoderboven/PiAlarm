#ifndef ALARMCONTROLLER_H
#define ALARMCONTROLLER_H

#include "model/AlarmsData.hpp"
#include "model/manager/AlarmManager.h"

/**
 * @namespace PiAlarm::controller
 * @brief Namespace for controllers in the PiAlarm application.
 */
namespace PiAlarm::controller {

    /**
     * @class AlarmController
     * @brief Controller for managing alarms in the PiAlarm application.
     *
     * This class provides an interface to interact with the model::AlarmsData,
     * allowing for setting, updating, and managing alarms.
     */
    class AlarmController {
        model::AlarmsData& alarmsData_;
        model::manager::AlarmManager& alarmManager_;

    public:

        /**
         * @brief Constructs an AlarmController with the specified alarms data.
         * @param alarmsData Reference to the model::AlarmsData instance to manage.
         * @param alarmManager Reference to the model::manager::AlarmManager instance that manage alarms.
         */
        explicit AlarmController(model::AlarmsData& alarmsData, model::manager::AlarmManager& alarmManager);

        /**
         * @brief Snoozes the currently ringing alarm.
         * This method will snooze the alarm for a predefined duration.
         * The duration is typically set in the AlarmManager.
         *
         * @note Do nothing if no alarm is currently ringing.
         * @note The snooze duration is defined in the AlarmManager and can be configured during
         *       the AlarmManager construction.
         */
        inline void snoozeAlarm() const;

        /**
         * @brief Stops the currently ringing alarm.
         * This method will stop the alarm immediately, regardless of its state.
         *
         * @note Do nothing if no alarm is currently ringing.
         */
        inline void stopAlarm() const;

    };

    // Inline methods implementations

    inline void AlarmController::snoozeAlarm() const {
        alarmManager_.snoozeAlarm();
    }

    inline void AlarmController::stopAlarm() const {
        alarmManager_.stopAlarm();
    }

} // namespace PiAlarm::controller

#endif //ALARMCONTROLLER_H
