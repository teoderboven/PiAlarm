#include "controller/AlarmController.h"

namespace PiAlarm::controller {

    AlarmController::AlarmController(model::AlarmsData& alarmsData)
        : alarmsData_{alarmsData}
    {}

} // namespace PiAlarm::controller
