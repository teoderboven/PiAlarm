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
         * @param temperature The temperature to set.
         * @param humidity The humidity to set.
         * @param valid True if the weather data is valid, false otherwise.
         */
        TemperatureSensorData(const float& temperature, const float& humidity, bool valid);

        /**
         * Sets the temperature and notifies observers of the change.
         * @param temperature The new temperature to set.
         */
        void setTemperature(const float& temperature);

        /**
         * Sets the humidity and notifies observers of the change.
         * @param humidity The new humidity to set.
         */
        void setHumidity(const float& humidity);

        /**
         * Sets the validity status of the weather data and notifies observers of the change.
         * @param valid True if the weather data is valid, false otherwise.
         */
        void setValid(bool valid);

        /**
         * Updates the weather data with new temperature, humidity, and validity status,
         * and notifies observers of the change.
         * @param temperature The new temperature to set.
         * @param humidity The new humidity to set.
         * @param valid True if the weather data is valid, false otherwise. Defaults to true.
         */
        void setValues(const float& temperature, const float& humidity, bool valid = true);

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
