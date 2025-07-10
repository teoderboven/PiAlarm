#ifndef ALARMCONTROLLER_H
#define ALARMCONTROLLER_H

#include "model/AlarmData.hpp"

/**
 * @namespace PiAlarm::controller
 * @brief Namespace for controllers in the PiAlarm application.
 */
namespace PiAlarm::controller {

    /**
     * @class AlarmController
     * @brief Controller for managing alarm data in the PiAlarm application.
     *
     * This class is responsible for handling operations related to alarm data.
     */
    class AlarmController {
        model::AlarmData& alarmData_;

    public:

        /**
         * @brief Constructs an AlarmController that manages alarm data.
         * @param alarmData Reference to the AlarmData model to be managed by this controller.
         */
        explicit AlarmController(model::AlarmData& alarmData);

    };

} // PiAlarm

#endif //ALARMCONTROLLER_H
