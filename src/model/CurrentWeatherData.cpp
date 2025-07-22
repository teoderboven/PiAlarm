#include "model/CurrentWeatherData.h"

namespace PiAlarm::model {

    CurrentWeatherData::CurrentWeatherData(
        const float& temperature,
        const float& humidity,
        const float& pressure,
        const common::WeatherCondition& condition,
        bool valid
        )
        : BaseModelData{},
        temperature_{temperature},
        humidity_{humidity},
        pressure_{pressure},
        condition_{condition},
        valid_{valid}
        {}

    void CurrentWeatherData::setTemperature(const float& temperature) {
        bool valueChanged = setIfDifferent(temperature_, temperature);

        if (valueChanged) notifyObservers();
    }

    void CurrentWeatherData::setHumidity(const float& humidity) {
        bool valueChanged = setIfDifferent(humidity_, humidity);

        if (valueChanged) notifyObservers();
    }

    void CurrentWeatherData::setPressure(const float& pressure) {
        bool valueChanged = setIfDifferent(pressure_, pressure);

        if (valueChanged) notifyObservers();
    }

    void CurrentWeatherData::setCondition(const common::WeatherCondition& condition) {
        bool valueChanged = setIfDifferent(condition_, condition);

        if (valueChanged) notifyObservers();
    }

    void CurrentWeatherData::setValid(bool valid) {
        bool valueChanged = setIfDifferent(valid_, valid);

        if (valueChanged) notifyObservers();
    }

    void CurrentWeatherData::setValues(
        const float& temperature,
        const float& humidity,
        const float& pressure,
        const common::WeatherCondition& condition,
        bool valid
    ) {
        bool valueChanged {false};

        {
            std::lock_guard lock{mutex_};

            const bool tempChanged = (temperature_ != temperature);
            const bool humChanged = (humidity_ != humidity);
            const bool pressChanged = (pressure_ != pressure);
            const bool condChanged = (condition_ != condition);
            const bool validChanged = (valid_ != valid);

            if ((tempChanged || humChanged || pressChanged || condChanged || validChanged) && (valid_ || valid)) {
                temperature_ = temperature;
                humidity_ = humidity;
                pressure_ = pressure;
                condition_ = condition;
                valid_ = valid;
                valueChanged = true;
            }
        }

        if (valueChanged) notifyObservers();
    }

} // namespace PiAlarm::model
