#include <chrono>

#include "model/AlarmsData.hpp"

namespace PiAlarm::model {

    AlarmsData::AlarmsData(std::size_t alarmCount)
        : Observable(), Observer(), alarms_{nullptr}, alarmCount_{alarmCount}
    {
        if (alarmCount_ == 0) {
            throw std::invalid_argument("AlarmsData requires at least one alarm.");
        }

        /*
         * Use the new[] operator because Alarm cannot be copied or moved (uses a mutex).
         * Because of this, we cannot use std::vector or std::deque.
         * std::array is also not suitable because it requires a compile-time constant size.
         */
        alarms_ = new Alarm[alarmCount];

        if (!loadFromFile(alarmCount)) {
            populateAlarms();

            getAlarm(0).setEnabled(); // Enable the first alarm by default
        }
    }

    AlarmsData::~AlarmsData() {
        delete[] alarms_;
        alarms_ = nullptr;
    }

    std::size_t AlarmsData::enabledAlarmCount() const noexcept {
        std::size_t count = 0;
        for (const auto &alarm : *this) {
            if (alarm.isEnabled()) {
                ++count;
            }
        }
        return count;
    }

    void AlarmsData::update() {
        saveToFile();
        notifyObservers();
    }

    void AlarmsData::setAlarm(std::size_t index, const Time &alarmTime, bool enabled) {
        getAlarm(index).setAlarm(alarmTime, enabled);
    }

    void AlarmsData::populateAlarms(std::size_t startIndex) const {
        for (auto i = startIndex; i < alarmCount_; ++i) {
            alarms_[i].setAlarm(Time{7, 0, 0}, false);
        }
    }

    bool AlarmsData::loadFromFile(std::size_t alarmsCount) {
        // TODO
        return false;
    }

    void AlarmsData::saveToFile() {
        // TODO
    }

} // namespace PiAlarm::model
