#ifndef ALARMDATA_HPP
#define ALARMDATA_HPP

#include <mutex>

#include "model/BaseModelData.hpp"
#include "model/Time.h"
#include "common/Observable.hpp"

namespace PiAlarm::model {

    /**
     * @class AlarmData
     * @brief Represents the data model for a clock alarm time and alarm status.
     *
     * This class extends the Observable class to notify observers of changes in the clock data.
     */
    class AlarmData final : public BaseModelData,  public common::Observable{
        Time alarmTime_;
        bool alarmEnabled_ = false;

    public:

        /**
         * Default constructor for AlarmData.
         */
        AlarmData() = default;

        /**
         * Constructs a AlarmData object with specified alarm time and alarm status.
         * @param alarm The alarm time to set.
         * @param enabled True if the alarm is enabled, false otherwise.
         */
        explicit AlarmData(const Time& alarm, bool enabled);

        /**
         * Sets the alarm time and notifies observers of the change.
         * @param time The new alarm time to set.
         */
        void setAlarmTime(const Time& time);

        /**
         * Enables or disables the alarm and notifies observers of the change.
         * @param enabled True to enable the alarm, false to disable it.
         */
        void setAlarmEnabled(bool enabled);

        /**
         * Sets both the alarm time and its enabled status, notifying observers of the change.
         * @param alarm The new alarm time to set.
         * @param enabled True to enable the alarm, false to disable it.
         */
        void setAlarm(const Time& alarm, bool enabled);

        /**
         * Gets the alarm time.
         * @return A reference to the alarm time.
         */
        [[nodiscard]]
        inline const Time& getAlarmTime() const;

        /**
         * Checks if the alarm is enabled.
         * @return A reference to the alarm enabled status.
         */
        [[nodiscard]]
        inline bool isAlarmEnabled() const;

    };

    // inline methods implementations

    inline const Time& AlarmData::getAlarmTime() const {
        std::lock_guard lock{mutex_};

        return alarmTime_;
    }

    inline bool AlarmData::isAlarmEnabled() const {
        std::lock_guard lock{mutex_};

        return alarmEnabled_;
    }

} // namespace PiAlarm::model

#endif //ALARMDATA_HPP
