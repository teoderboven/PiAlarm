#ifndef CURRENT_INDOOR_DATA_HPP
#define CURRENT_INDOOR_DATA_HPP

#include <mutex>

#include "model/BaseModelData.hpp"
#include "common/Observable.hpp"

namespace PiAlarm::model {

    /**
     * @class CurrentIndoorData
     * @brief Represents the data from a temperature sensor, including temperature, humidity, pressure, and validity status.
     *
     * This class extends the Observable class to notify observers of changes in the temperature sensor data.
     */
    class CurrentIndoorData final : public BaseModelData, public common::Observable {
        float temperature_;
        float humidity_;
        float pressure_;
        bool valid_ = false;

    public:
        /**
         * @brief Default constructor for CurrentIndoorData.
         */
        CurrentIndoorData() = default;

        /**
         * @brief Constructs a CurrentIndoorData object with specified temperature, humidity, pressure, and validity status.
         * @param temperature The temperature to set.
         * @param humidity The humidity to set.
         * @param pressure The pressure to set in hPa.
         * @param valid True if the weather data is valid, false otherwise.
         */
        CurrentIndoorData(float temperature, float humidity, float pressure, bool valid);

        /**
         * @brief Sets the temperature and notifies observers of the change.
         * @param temperature The new temperature to set.
         */
        void setTemperature(float temperature);

        /**
         * @brief Sets the humidity and notifies observers of the change.
         * @param humidity The new humidity to set.
         */
        void setHumidity(float humidity);

        /**
         * @brief Sets the pressure and notifies observers of the change.
         * @param pressure The new pressure to set in hPa.
         */
        void setPressure(float pressure);

        /**
         * @brief Sets the validity status of the weather data and notifies observers of the change.
         * @param valid True if the weather data is valid, false otherwise.
         */
        void setValid(bool valid);

        /**
         * @brief Updates the weather data with new temperature, humidity, pressure, and validity status,
         * and notifies observers of the change.
         * @param temperature The new temperature to set.
         * @param humidity The new humidity to set.
         * @param pressure The new pressure to set in hPa.
         * @param valid True if the weather data is valid, false otherwise. Defaults to true.
         */
        void setValues(float temperature, float humidity, float pressure, bool valid = true);

        /**
         * @brief Gets the current temperature.
         * @return The current temperature.
         * @note Always checks if the weather data is valid before using this value.
         */
        [[nodiscard]]
        inline float getTemperature() const;

        /**
         * @brief Gets the current humidity.
         * @return The current humidity.
         * @note Always checks if the weather data is valid before using this value.
         */
        [[nodiscard]]
        inline float getHumidity() const;

        /**
         * @brief Gets the current pressure.
         * @return The current pressure in hPa.
         * @note Always checks if the weather data is valid before using this value.
         */
        [[nodiscard]]
        inline float getPressure() const;

        /**
         * @brief Checks if the weather data is valid.
         * Invalid data may indicate that the weather service is unavailable or that the data could not be fetched.
         * @return True if the weather data is valid, false otherwise.
         */
        [[nodiscard]]
        inline bool isValid() const;

    };

    // inline methods implementations

    inline float CurrentIndoorData::getTemperature() const {
        std::lock_guard lock{mutex_};

        return temperature_;
    }

    inline float CurrentIndoorData::getHumidity() const {
        std::lock_guard lock{mutex_};

        return humidity_;
    }

    inline float CurrentIndoorData::getPressure() const {
        std::lock_guard lock{mutex_};

        return pressure_;
    }

    inline bool CurrentIndoorData::isValid() const {
        std::lock_guard lock{mutex_};

        return valid_;
    }

} // namespace PiAlarm::model

#endif //CURRENT_INDOOR_DATA_HPP