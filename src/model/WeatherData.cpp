#include "model/WeatherData.hpp"

namespace PiAlarm::model {

    WeatherData::WeatherData(const float& temp, const float& hum, bool isValid)
        : temperature_{temp}, humidity_{hum}, valid_{isValid}
    {}

    void WeatherData::setTemperature(const float& temp) {
        bool shouldNotify {false};

        {
            std::lock_guard lock{mutex_};
            if (temperature_ != temp) {
                temperature_ = temp;
                shouldNotify = true;
            }
        }

        if (shouldNotify) notifyObservers();
    }

    void WeatherData::setHumidity(const float& hum) {
        bool shouldNotify {false};

        {
            std::lock_guard lock{mutex_};
            if (humidity_ != hum) {
                humidity_ = hum;
                shouldNotify = true;
            }
        }

        if (shouldNotify) notifyObservers();
    }

    void WeatherData::setValid(const bool& isValid) {
        bool shouldNotify {false};

        {
            std::lock_guard lock{mutex_};
            if (valid_ != isValid) {
                valid_ = isValid;
                shouldNotify = true;
            }
        }

        if (shouldNotify) notifyObservers();
    }

    void WeatherData::updateWeather(const float& temp, const float& hum, bool isValid) {
        bool shouldNotify = false;

        {
            std::lock_guard lock{mutex_};

            const bool tempChanged = (temperature_ != temp);
            const bool humChanged = (humidity_ != hum);
            const bool validChanged = (valid_ != isValid);

            const bool anyChange = tempChanged || humChanged || validChanged;

            // Only update if at least one state is valid
            // Prevents updating when both old and new states are invalid
            const bool anyValid = valid_ || isValid;

            if (anyChange && anyValid) {
                temperature_ = temp;
                humidity_ = hum;
                valid_ = isValid;
                shouldNotify = true;
            }
        }

        if (shouldNotify) notifyObservers();
    }

} // namespace PiAlarm::model
