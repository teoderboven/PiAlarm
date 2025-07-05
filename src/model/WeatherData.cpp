#include "model/WeatherData.hpp"

namespace PiAlarm::model {

    WeatherData::WeatherData(const float& temp, const float& hum, bool isValid)
        : temperature_{temp}, humidity_{hum}, valid_{isValid}
    {}

    void WeatherData::setTemperature(const float& temp) {
        std::lock_guard lock{mutex_};

        temperature_ = temp;
        notifyObservers();
    }

    void WeatherData::setHumidity(const float& hum) {
        std::lock_guard lock{mutex_};

        humidity_ = hum;
        notifyObservers();
    }

    void WeatherData::setValid(const bool& isValid) {
        std::lock_guard lock{mutex_};

        valid_ = isValid;
        notifyObservers();
    }

    void WeatherData::updateWeather(const float& temp, const float& hum, bool isValid) {
        std::lock_guard lock{mutex_};

        temperature_ = temp;
        humidity_ = hum;
        valid_ = isValid;
        notifyObservers();
    }

} // namespace PiAlarm::model
