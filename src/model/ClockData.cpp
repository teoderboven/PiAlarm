#include "model/ClockData.hpp"

namespace PiAlarm::model {

    ClockData::ClockData(const Time& current)
        : currentTime_{current}
    {}

    void ClockData::setCurrentTime(const Time& time) {
        bool shouldNotify {false};

        {
            std::lock_guard lock{mutex_};
            if (currentTime_ != time) {
                currentTime_ = time;
                shouldNotify = true;
            }
        }

        if (shouldNotify) notifyObservers();
    }

} // namespace PiAlarm::model
