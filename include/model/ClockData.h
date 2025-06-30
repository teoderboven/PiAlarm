#ifndef CLOCKDATA_H
#define CLOCKDATA_H

#include "model/Time.h"

namespace PiAlarm::model {

    /**
     * @struct ClockData
     * Holds clock-related data.
     *
     * This structure stores the current time, the alarm time,
     * and whether the alarm is enabled.
     */
    struct ClockData {
        /** Current time */
        Time currentTime;

        /** Alarm time */
        Time alarmTime;

        /** Alarm enabled state (true = enabled, false = disabled) */
        bool alarmEnabled = false;
    };


} // namespace PiAlarm::model

#endif //CLOCKDATA_H
