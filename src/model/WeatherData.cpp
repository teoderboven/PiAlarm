#include "model/WeatherData.hpp"

namespace PiAlarm::model {

    WeatherData::WeatherData(const float& temp, const float& hum, bool isValid)
        : temperature_{temp}, humidity_{hum}, valid_{isValid}
    {}

    void WeatherData::setTemperature(const float& temp) {
        temperature_ = temp;
        notifyObservers();
    }

    void WeatherData::setHumidity(const float& hum) {
        humidity_ = hum;
        notifyObservers();
    }

    void WeatherData::setValid(const bool& isValid) {
        valid_ = isValid;
        notifyObservers();
    }

    void WeatherData::updateWeather(const float& temp, const float& hum, bool isValid) {
        temperature_ = temp;
        humidity_ = hum;
        valid_ = isValid;
        notifyObservers();
    }

} // namespace PiAlarm::model
