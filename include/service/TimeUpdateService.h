#ifndef TIMEUPDATESERVICE_H
#define TIMEUPDATESERVICE_H

#include "service/BaseService.h"
#include "model/ClockData.hpp"

namespace PiAlarm::service {

    /**
     * @class TimeUpdateService
     * @brief Service for updating the current time in the ClockData model.
     *
     * This service updates the current time in the ClockData model every second,
     * ensuring that the time is always accurate and synchronized with the system clock.
     */
    class TimeUpdateService final : public BaseService {
        model::ClockData &clockData_; ///< Reference to the ClockData model to update time

    public:

        /**
         * @brief Constructs a TimeUpdateService that updates the current time in the ClockData model.
         * @param clockData Reference to the ClockData model to be updated.
         */
        explicit TimeUpdateService(model::ClockData &clockData);

        /**
         * @brief Updates the current time in the ClockData model.
         */
        void update() override;

        /**
         * @brief Waits until the next second to ensure precise time updates.
         *
         * This method uses a time_point to wait until the start of the next second,
         * ensuring that the time is updated exactly once per second without drift.
         */
        void waitNextCycle() override;

    };

} // namespace PiAlarm::service

#endif //TIMEUPDATESERVICE_H
