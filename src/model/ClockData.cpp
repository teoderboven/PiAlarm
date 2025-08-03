#include "model/ClockData.hpp"

namespace PiAlarm::model {

    ClockData::ClockData(Time current)
        : currentTime_{current}
    {}

    void ClockData::setCurrentTime(Time time) {
        bool valueChanged = setIfDifferent(currentTime_, time);

        if (valueChanged) notifyObservers();
    }

} // namespace PiAlarm::model
