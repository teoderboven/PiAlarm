#include "model/TemperatureSensorData.hpp"

namespace PiAlarm::model {

    TemperatureSensorData::TemperatureSensorData(const float& temperature, const float& humidity, bool valid)
        : temperature_{temperature}, humidity_{humidity}, valid_{valid}
    {}

    void TemperatureSensorData::setTemperature(const float& temperature) {
        bool shouldNotify {false};

        {
            std::lock_guard lock{mutex_};
            if (temperature_ != temperature) {
                temperature_ = temperature;
                shouldNotify = true;
            }
        }

        if (shouldNotify) notifyObservers();
    }

    void TemperatureSensorData::setHumidity(const float& humidity) {
        bool shouldNotify {false};

        {
            std::lock_guard lock{mutex_};
            if (humidity_ != humidity) {
                humidity_ = humidity;
                shouldNotify = true;
            }
        }

        if (shouldNotify) notifyObservers();
    }

    void TemperatureSensorData::setValid(bool valid) {
        bool shouldNotify {false};

        {
            std::lock_guard lock{mutex_};
            if (valid_ != valid) {
                valid_ = valid;
                shouldNotify = true;
            }
        }

        if (shouldNotify) notifyObservers();
    }

    void TemperatureSensorData::setValues(const float& temperature, const float& humidity, bool valid) {
        bool shouldNotify = false;

        {
            std::lock_guard lock{mutex_};

            const bool tempChanged = (temperature_ != temperature);
            const bool humChanged = (humidity_ != humidity);
            const bool validChanged = (valid_ != valid);

            const bool anyChange = tempChanged || humChanged || validChanged;

            // Only update if at least one state is valid
            // Prevents updating when both old and new states are invalid
            const bool anyValid = valid_ || valid;

            if (anyChange && anyValid) {
                temperature_ = temperature;
                humidity_ = humidity;
                valid_ = valid;
                shouldNotify = true;
            }
        }

        if (shouldNotify) notifyObservers();
    }

} // namespace PiAlarm::model
