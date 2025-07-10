#include "controller/AlarmController.h"

namespace PiAlarm::controller {

    AlarmController::AlarmController(model::AlarmData &alarmData)
        : alarmData_{alarmData}
    {
        alarmData_.setAlarm(model::Time(7, 0, 0), true); // Default alarm time set to 7:00 AM
    }

} // namespace PiAlarm::controller
