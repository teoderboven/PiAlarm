#include "controller/AlarmController.h"

namespace PiAlarm::controller {

    AlarmController::AlarmController(model::AlarmsData& alarmsData, model::manager::AlarmManager& alarmManager)
        : alarmsData_{alarmsData}, alarmManager_{alarmManager}
    {}

    std::optional<std::string> AlarmController::setAlarm(size_t index, int hour, int minute, bool enabled) const {
        try {
            alarmsData_.setAlarm(index, model::Time{hour, minute}, enabled);
            return {};
        } catch (...) {
            return std::make_optional<std::string>("Failed to set alarm");
        }
    }

} // namespace PiAlarm::controller
