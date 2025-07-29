#include "controller/AlarmController.h"

namespace PiAlarm::controller {

    AlarmController::AlarmController(model::AlarmsData& alarmsData, model::manager::AlarmManager& alarmManager)
        : alarmsData_{alarmsData}, alarmManager_{alarmManager}
    {}

} // namespace PiAlarm::controller
