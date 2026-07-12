#pragma once

#ifdef SENSOR_SCD41

#include "service/BaseService.h"
#include "hardware/SCD41.h"
#include "model/CO2Data.hpp"
#include "model/CurrentIndoorData.hpp"

namespace PiAlarm::service {

    /**
     * @class SCD41Service
     * @brief Service for fetching and updating indoor environmental data from the SCD41 sensor.
     */
    class SCD41Service : public BaseService {
        hardware::SCD41 scd41_; ///< SCD41 sensor for indoor measurements.
        model::CO2Data& co2Data_; ///< Reference to the CO2Data model to be updated with sensor readings.
        const model::CurrentIndoorData& currentIndoorData_; ///< Reference to the CurrentIndoorData model to be notified when pressure changes.

    public:
        /**
         * @brief Constructs a CurrentIndoorService that updates the CO2Data model with sensor data.
         * @param CO2Data Reference to the CO2Data model to be updated with sensor readings.
         * @param currentIndoorData Reference to the CurrentIndoorData model to be notified when pressure changes.
         */
        explicit SCD41Service(model::CO2Data& CO2Data, const model::CurrentIndoorData& currentIndoorData);

    protected:
        /**
         * @brief Initializes the SCD41 sensor for periodic measurement.
         * @return true if the sensor was successfully initialized, false otherwise.
         */
        bool onStart() override;

        /**
         * @brief Stops the SCD41 sensor's periodic measurement.
         */
        void onStop() override;

        /**
         * @brief Updates the data in the CO2Data model.
         */
        void process() override;

        /**
         * @brief Returns the update interval for the service.
         * @return The duration until the next reading.
         */
        std::chrono::milliseconds updateInterval() const override;
    };

} // namespace PiAlarm::service

#endif // SENSOR_SCD41