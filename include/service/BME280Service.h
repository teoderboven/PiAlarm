#ifndef PIALARM_BME280SERVICE_H
#define PIALARM_BME280SERVICE_H

#ifdef SENSOR_BME280

#include "service/BaseService.h"
#include "hardware/BME280.h"
#include "model/CurrentIndoorData.hpp"

namespace PiAlarm::service {

    /**
     * @class BME280Service
     * @brief Service for fetching and updating indoor environmental data from the BME280 sensor.
     */
    class BME280Service : public BaseService {
        hardware::BME280 BME280_; ///< BME280 sensor for indoor measurements.
        model::CurrentIndoorData& currentIndoorData_; ///< Reference to the CurrentIndoorData model to be updated with sensor readings.
        std::chrono::milliseconds measurementDelay_ {50}; ///< Delay required by the BME280 sensor between measurements. Set to 50 by default, but updated after the initialization of the sensor.

    public:
        /**
         * @brief Constructs a CurrentIndoorService that updates the CurrentIndoorData model with indoor sensor data.
         * @param currentIndoorData Reference to the CurrentIndoorData model to be updated with sensor readings.
         */
        explicit BME280Service(model::CurrentIndoorData& currentIndoorData);

    protected:
        /**
         * @brief Initializes the BME280 sensor.
         * @return true if the sensor was successfully initialized, false otherwise.
         */
        bool onStart() override;

        /**
         * @brief Updates the data in the CurrentIndoorData model.
         */
        void update() override;

        /**
         * @brief Returns the update interval for the service.
         * @return The duration until the next reading.
         */
        std::chrono::milliseconds updateInterval() const override;
    };

} // namespace PiAlarm::service

#endif // SENSOR_BME280
#endif //PIALARM_BME280SERVICE_H
