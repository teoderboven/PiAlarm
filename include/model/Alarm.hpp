#ifndef ALARMDATA_HPP
#define ALARMDATA_HPP

#include <mutex>

#include "model/BaseModelData.hpp"
#include "model/Time.h"
#include "common/Observable.hpp"

namespace PiAlarm::model {

    /**
     * @class Alarm
     * @brief Represents the data model for a clock alarm time and alarm status.
     *
     * This class extends the Observable class to notify observers of changes in the clock data.
     */
    class Alarm final : public BaseModelData,  public common::Observable{
        Time alarmTime_;
        bool alarmEnabled_ = false;

    public:

        /**
         * Default constructor for Alarm.
         */
        Alarm() = default;

        /**
         * Constructs an Alarm object with specified alarm time and alarm status.
         * @param alarm The alarm time to set.
         * @param enabled True if the alarm is enabled, false otherwise.
         */
        explicit Alarm(const Time& alarm, bool enabled);

        /**
         * Sets the alarm time and notifies observers of the change.
         * @param time The new alarm time to set.
         */
        void setTime(const Time& time);

        /**
         * Enables or disables the alarm and notifies observers of the change.
         * @param enabled True to enable the alarm, false to disable it.
         */
        void setEnabled(bool enabled = true);

        /**
         * @brief Disables the alarm and notifies observers of the change.
         * This is a convenience method that calls setEnabled(false).
         */
        inline void setDisabled();

        /**
         * Sets both the alarm time and its enabled status, notifying observers of the change.
         * @param alarm The new alarm time to set.
         * @param enabled True to enable the alarm, false to disable it.
         */
        void setAlarm(const Time& alarm, bool enabled);

        /**
         * Gets the current alarm time.
         * @return A const reference to the alarm time.
         */
        [[nodiscard]]
        inline const Time& getTime() const;

        /**
         * Checks if the alarm is enabled.
         * @return True if the alarm is enabled, false otherwise.
         */
        [[nodiscard]]
        inline bool isEnabled() const;

    };

    // inline methods implementations

    inline void Alarm::setDisabled() {
        setEnabled(false);
    }

    inline const Time& Alarm::getTime() const {
        std::lock_guard lock{mutex_};

        return alarmTime_;
    }

    inline bool Alarm::isEnabled() const {
        std::lock_guard lock{mutex_};

        return alarmEnabled_;
    }

} // namespace PiAlarm::model

#endif //ALARMDATA_HPP
