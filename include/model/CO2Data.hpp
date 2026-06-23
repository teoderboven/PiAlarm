#ifndef CO2_DATA_HPP
#define CO2_DATA_HPP

#include <mutex>

#include "model/BaseModelData.hpp"
#include "common/Observable.hpp"

namespace PiAlarm::model {

    /**
     * @enum AirQualityLevel
     * @brief Enumeration for different air quality levels based on CO2 concentration.
     */
    enum class AirQualityLevel : uint8_t {
        Unknown,
        Good,
        Poor,
        VeryPoor
    };

    /**
     * @class CO2Data
     * @brief Represents the CO2 level data, including CO2 concentration and air quality level.
     *
     * This class extends the Observable class to notify observers of changes in the CO2 data.
     */
    class CO2Data final : public BaseModelData, public common::Observable {
        uint16_t co2_ppm_;
        bool valid_ = false;

    public:
        /**
         * @brief Default constructor for CO2Data.
         */
        CO2Data() = default;

        /**
         * @brief Constructs a CO2Data object with specified CO2 concentration and validity status.
         * @param co2_ppm The CO2 concentration in ppm (parts per million) to set.
         * @param valid True if the CO2 data is valid, false otherwise.
         */
        CO2Data(uint16_t co2_ppm, bool valid);

        /**
         * @brief Sets the CO2 concentration and notifies observers of the change.
         * @param co2_ppm The new CO2 concentration in ppm to set.
         */
        void setCO2(uint16_t co2_ppm);

        /**
         * @brief Sets the validity status of the CO2 data and notifies observers of the change.
         * @param valid True if the CO2 data is valid, false otherwise.
         */
        void setValid(bool valid);

        /**
         * @brief Updates the CO2 data with new concentration and validity status,
         * and notifies observers of the change.
         * @param co2_ppm The new CO2 concentration in ppm to set.
         * @param valid True if the CO2 data is valid, false otherwise. Defaults to true.
         */
        void setValues(uint16_t co2_ppm, bool valid = true);

        /**
         * @brief Gets the current CO2 concentration in ppm.
         * @return The current CO2 concentration.
         * @note Always check if the CO2 data is valid before using this value.
         */
        [[nodiscard]]
        inline uint16_t getCO2() const;

        /**
         * @brief Gets the air quality level based on the current CO2 concentration.
         * @return The air quality level as an AirQualityLevel enum value.
         * @note Always check if the CO2 data is valid before using this value.
         */
        [[nodiscard]]
        inline AirQualityLevel getAirQualityLevel() const;

        /**
         * @brief Checks if the CO2 data is valid.
         * Invalid data may indicate that the CO2 sensor is unavailable or that the data could not be fetched.
         * @return True if the CO2 data is valid, false otherwise.
         */
        [[nodiscard]]
        inline bool isValid() const;

    };

    // inline methods implementations

    inline uint16_t CO2Data::getCO2() const {
        std::lock_guard lock{mutex_};

        return co2_ppm_;
    }

    inline AirQualityLevel CO2Data::getAirQualityLevel() const {
        std::lock_guard lock{mutex_};

        if (co2_ppm_ < 800) return AirQualityLevel::Good;
        if (co2_ppm_ < 1500) return AirQualityLevel::Poor;
        return AirQualityLevel::VeryPoor;
    }

    inline bool CO2Data::isValid() const {
        std::lock_guard lock{mutex_};

        return valid_;
    }

} // namespace PiAlarm::model

#endif //CO2_DATA_HPP