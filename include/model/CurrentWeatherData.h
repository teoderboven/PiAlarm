#ifndef CURRENTWEATHERDATA_H
#define CURRENTWEATHERDATA_H

#include "model/BaseModelData.hpp"
#include "common/Observable.hpp"
#include "common/WeatherCondition.h"

namespace PiAlarm::model {

    /**
     * @class CurrentWeatherData
     * @brief Represents the current weather data including temperature, humidity, pressure, and condition.
     *
     * This class extends the Observable class to notify observers of changes in the weather data.
     */
    class CurrentWeatherData final : public BaseModelData, public common::Observable {
        float temperature_;
        float humidity_;
        float pressure_;
        common::WeatherCondition condition_;
        bool valid_ = false;

    public:

        /**
         * Default constructor for CurrentWeatherData.
         */
        CurrentWeatherData() = default;

        /**
         * Constructs a CurrentWeatherData object with specified temperature, humidity, pressure, condition and validity status.
         * @param temperature The temperature to set.
         * @param humidity The humidity to set.
         * @param pressure The pressure to set.
         * @param condition The weather condition to set.
         * @param valid True if the weather data is valid, false otherwise.
         */
        CurrentWeatherData(float temperature, float humidity, float pressure, common::WeatherCondition condition, bool valid);

        /**
         * Sets the current temperature and notifies observers of the change.
         * @param temperature The new temperature to set.
         */
        void setTemperature(float temperature);

        /**
         * Sets the current humidity and notifies observers of the change.
         * @param humidity The new humidity to set.
         */
        void setHumidity(float humidity);

        /**
         * Sets the current pressure and notifies observers of the change.
         * @param pressure The new pressure to set.
         */
        void setPressure(float pressure);

        /**
         * Sets the current weather condition and notifies observers of the change.
         * @param condition The new weather condition to set.
         */
        void setCondition(common::WeatherCondition condition);

        /**
         * Sets the validity status of the weather data and notifies observers of the change.
         * @param valid True if the weather data is valid, false otherwise.
         */
        void setValid(bool valid);

        /**
         * Updates the weather data with new temperature, humidity, pressure, condition, and validity status,
         * and notifies observers of the change.
         * @param temperature The new temperature to set.
         * @param humidity The new humidity to set.
         * @param pressure The new pressure to set.
         * @param condition The new weather condition to set.
         * @param valid True if the weather data is valid, false otherwise. Defaults to true.
         */
        void setValues(float temperature, float humidity, float pressure, common::WeatherCondition condition, bool valid = true);

        /**
         * Gets the current temperature.
         * @return A reference to the current temperature.
         * @note Always checks if the weather data is valid before using this value.
         */
        [[nodiscard]]
        inline float getTemperature() const;

        /**
         * Gets the current humidity.
         * @return A reference to the current humidity.
         * @note Always checks if the weather data is valid before using this value.
         */
        [[nodiscard]]
        inline float getHumidity() const;

        /**
         * Gets the current pressure.
         * @return A reference to the current pressure.
         * @note Always checks if the weather data is valid before using this value.
         */
        [[nodiscard]]
        inline float getPressure() const;

        /**
         * Gets the current weather condition.
         * @return A reference to the current weather condition.
         * @note Always checks if the weather data is valid before using this value.
         */
        [[nodiscard]]
        inline const common::WeatherCondition& getCondition() const;

        /**
         * Checks if the weather data is valid.
         * @return True if the weather data is valid, false otherwise.
         */
        [[nodiscard]]
        inline bool isValid() const;

    };

    // Inline method implementations

    inline float CurrentWeatherData::getTemperature() const {
        std::lock_guard lock{mutex_};

        return temperature_;
    }

    inline float CurrentWeatherData::getHumidity() const {
        std::lock_guard lock{mutex_};

        return humidity_;
    }

    inline float CurrentWeatherData::getPressure() const {
        std::lock_guard lock{mutex_};

        return pressure_;
    }

    inline const common::WeatherCondition& CurrentWeatherData::getCondition() const {
        std::lock_guard lock{mutex_};

        return condition_;
    }

    inline bool CurrentWeatherData::isValid() const {
        std::lock_guard lock{mutex_};

        return valid_;
    }

} // namespace PiAlarm::model

#endif //CURRENTWEATHERDATA_H
