#ifndef CLOCKDATA_HPP
#define CLOCKDATA_HPP

#include <mutex>

#include "model/BaseModelData.hpp"
#include "model/Time.h"
#include "common/Observable.hpp"

namespace PiAlarm::model {

    /**
     * @class ClockData
     * @brief Represents the data model for a clock
     *
     * This class extends the Observable class to notify observers of changes in the clock data.
     */
    class ClockData final : public BaseModelData, public common::Observable{
        Time currentTime_;

    public:

        /**
         * Default constructor for ClockData.
         */
        ClockData() = default;

        /**
         * Constructs a ClockData object with specified current time, alarm time, and alarm status.
         * @param current The current time to set.
         */
        explicit ClockData(Time current);

        /**
         * Sets the current time and notifies observers of the change.
         * @param time The new current time to set.
         */
        void setCurrentTime(Time time);

        /**
         * Gets the current time.
         * @return A reference to the current time.
         */
        [[nodiscard]]
        inline Time getCurrentTime() const;

    };

    // inline method implementation

    inline Time ClockData::getCurrentTime() const {
        std::lock_guard lock{mutex_};

        return currentTime_;
    }

} // namespace PiAlarm::model

#endif //CLOCKDATA_HPP
