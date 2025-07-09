#ifndef TEMPERATURESENSORDATA_HPP
#define TEMPERATURESENSORDATA_HPP

#include <mutex>

#include "common/Observable.hpp"

namespace PiAlarm::model {

    /**
     * @class TemperatureSensorData
     * @brief Represents the data from a temperature sensor, including temperature, humidity, and validity status.
     *
     * This class extends the Observable class to notify observers of changes in the temperature sensor data.
     */
    class TemperatureSensorData final : public common::Observable {
        float temperature_;
        float humidity_;
        bool valid_ = false;

        // mutable to allow usage of mutex_ in const methods
        mutable std::mutex mutex_; ///< Protects access to data (multithreading)

    public:
        /**
         * Default constructor for TemperatureSensorData.
         */
        TemperatureSensorData() = default;

        /**
         * Constructs a TemperatureSensorData object with specified temperature, humidity, and validity status.
         * @param temp The temperature to set.
         * @param hum The humidity to set.
         * @param isValid True if the weather data is valid, false otherwise.
         */
        TemperatureSensorData(const float& temp, const float& hum, bool isValid);

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
        void setValues(const float& temp, const float& hum, bool isValid);

        /**
         * Gets the current temperature.
         * @return The current temperature.
         * @note Always checks if the weather data is valid before using this value.
         */
        [[nodiscard]]
        inline const float& getTemperature() const;

        /**
         * Gets the current humidity.
         * @return The current humidity.
         * @note Always checks if the weather data is valid before using this value.
         */
        [[nodiscard]]
        inline const float& getHumidity() const;

        /**
         * Checks if the weather data is valid.
         * Invalid data may indicate that the weather service is unavailable or that the data could not be fetched.
         * @return True if the weather data is valid, false otherwise.
         */
        [[nodiscard]]
        inline bool isValid() const;

    };

    // inline methods implementations

    inline const float& TemperatureSensorData::getTemperature() const {
        std::lock_guard lock{mutex_};

        return temperature_;
    }

    inline const float& TemperatureSensorData::getHumidity() const {
        std::lock_guard lock{mutex_};

        return humidity_;
    }

    inline bool TemperatureSensorData::isValid() const {
        std::lock_guard lock{mutex_};

        return valid_;
    }

} // namespace PiAlarm::model

#endif //TEMPERATURESENSORDATA_HPP
