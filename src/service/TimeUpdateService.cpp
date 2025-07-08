#include <chrono>

#include "service/TimeUpdateService.h"
#include "model/Time.h"

namespace PiAlarm::service {

    TimeUpdateService::TimeUpdateService(model::ClockData &clockData)
        : BaseService(), clockData_{clockData}
    {}

    void TimeUpdateService::update() {
        clockData_.setCurrentTime(model::Time::now());
    }

    void TimeUpdateService::waitNextCycle() {
        using namespace std::chrono;

        const auto now = system_clock::now();

        // use a time_point to specify the exact next second instant to wait until,
        // ensuring precise synchronization with the system clock,
        // rather than waiting a fixed duration which can accumulate drift.
        auto next_tick = time_point_cast<seconds>(now) + seconds{1};

        // if we’re already past next tick (possible with low-precision clocks), add another second
        if (next_tick <= now) {
            next_tick += seconds{1};
        }

        std::this_thread::sleep_until(next_tick);
    }

} // namespace PiAlarm::service
