#include "model/TemperatureSensorData.hpp"

namespace PiAlarm::model {

    TemperatureSensorData::TemperatureSensorData(float temperature, float humidity, bool valid)
        : temperature_{temperature}, humidity_{humidity}, valid_{valid}
    {}

    void TemperatureSensorData::setTemperature(float temperature) {
        bool valueChanged = setIfDifferent(temperature_, temperature);

        if (valueChanged) notifyObservers();
    }

    void TemperatureSensorData::setHumidity(float humidity) {
        bool valueChanged = setIfDifferent(humidity_, humidity);

        if (valueChanged) notifyObservers();
    }

    void TemperatureSensorData::setValid(bool valid) {
        bool valueChanged = setIfDifferent(valid_, valid);

        if (valueChanged) notifyObservers();
    }

    void TemperatureSensorData::setValues(float temperature, float humidity, bool valid) {
        bool valueChanged = false;

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
                valueChanged = true;
            }
        }

        if (valueChanged) notifyObservers();
    }

} // namespace PiAlarm::model
