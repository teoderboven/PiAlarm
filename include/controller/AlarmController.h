#ifndef ALARMCONTROLLER_H
#define ALARMCONTROLLER_H

#include "model/AlarmsData.hpp"

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

    public:

        /**
         * @brief Constructs an AlarmController with the specified alarms data.
         * @param alarmsData Reference to the model::AlarmsData instance to manage.
         */
        explicit AlarmController(model::AlarmsData& alarmsData);

    };

} // namespace PiAlarm::controller

#endif //ALARMCONTROLLER_H
