#ifndef WEATHERDATA_HPP
#define WEATHERDATA_HPP

#include "common/Observable.hpp"

namespace PiAlarm::model {

    /**
     * @class WeatherData
     * @brief Represents the data model for weather information, including temperature, humidity, and validity status.
     *
     * This class extends the Observable class to notify observers of changes in the weather data.
     */
    class WeatherData final : public common::Observable {
        float temperature_;
        float humidity_;
        bool valid_ = false;

    public:
        /**
         * Default constructor for WeatherData.
         */
        WeatherData() = default;

        /**
         * Constructs a WeatherData object with specified temperature, humidity, and validity status.
         * @param temp The temperature to set.
         * @param hum The humidity to set.
         * @param isValid True if the weather data is valid, false otherwise.
         */
        WeatherData(const float& temp, const float& hum, bool isValid);

        /**
         * Sets the temperature and notifies observers of the change.
         * @param temp The new temperature to set.
         */
        void setTemperature(const float& temp);

        /**
         * Sets the humidity and notifies observers of the change.
         * @param hum The new humidity to set.
         */
        void setHumidity(const float& hum);

        /**
         * Sets the validity status of the weather data and notifies observers of the change.
         * @param isValid True if the weather data is valid, false otherwise.
         */
        void setValid(const bool& isValid);

        /**
         * Updates the weather data with new temperature, humidity, and validity status,
         * and notifies observers of the change.
         * @param temp The new temperature to set.
         * @param hum The new humidity to set.
         * @param isValid True if the weather data is valid, false otherwise.
         */
        void updateWeather(const float& temp, const float& hum, bool isValid);

        /**
         * Gets the current temperature.
         * @return The current temperature.
         * @note Always checks if the weather data is valid before using this value.
         */
        [[nodiscard]]
        const float& getTemperature() const {
            return temperature_;
        }

        /**
         * Gets the current humidity.
         * @return The current humidity.
         * @note Always checks if the weather data is valid before using this value.
         */
        [[nodiscard]]
        const float& getHumidity() const {
            return humidity_;
        }

        /**
         * Checks if the weather data is valid.
         * Invalid data may indicate that the weather service is unavailable or that the data could not be fetched.
         * @return True if the weather data is valid, false otherwise.
         */
        [[nodiscard]]
        bool isValid() const {
            return valid_;
        }

    };

} // namespace PiAlarm::model

#endif //WEATHERDATA_HPP
