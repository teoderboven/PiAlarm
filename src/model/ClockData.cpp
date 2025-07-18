#include "model/ClockData.hpp"

namespace PiAlarm::model {

    ClockData::ClockData(const Time& current)
        : currentTime_{current}
    {}

    void ClockData::setCurrentTime(const Time& time) {
        bool valueChanged = setIfDifferent(currentTime_, time);

        if (valueChanged) notifyObservers();
    }

} // namespace PiAlarm::model
