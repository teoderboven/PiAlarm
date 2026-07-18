#include "AlarmController.h"

namespace PiAlarm::controller {

    AlarmController::AlarmController(model::AlarmsData& alarmsData, model::manager::AlarmManager& alarmManager)
        : alarmsData_{alarmsData}, alarmManager_{alarmManager}
    {}

    bool AlarmController::setAlarm(size_t index, int hour, int minute, bool enabled) const {
        try {
            alarmsData_.setAlarm(index, model::Time{hour, minute}, enabled);
            return true;
        } catch (...) {
            return false;
        }
    }

} // namespace PiAlarm::controller
