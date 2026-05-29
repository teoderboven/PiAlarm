#ifndef PIALARM_CURRENTINDOORSERVICE_H
#define PIALARM_CURRENTINDOORSERVICE_H

#include "service/BaseService.h"
#include "hardware/SCD41.h"
#include "model/CurrentIndoorData.hpp"

namespace PiAlarm::service {

    /**
     * @class CurrentIndoorService
     * @brief Service for fetching and updating indoor environmental data from the SCD41 sensor.
     */
    class CurrentIndoorService : public BaseService {
        const hardware::SCD41 sensor; ///< SCD41 sensor for indoor measurements.
        model::CurrentIndoorData& currentIndoorData; ///< Reference to the CurrentIndoorData model to be updated with sensor readings.

    public:
        /**
         * @brief Constructs a CurrentIndoorService that updates the CurrentIndoorData model with indoor sensor data.
         * @param currentIndoorData Reference to the CurrentIndoorData model to be updated with sensor readings.
         */
        explicit CurrentIndoorService(model::CurrentIndoorData& currentIndoorData);

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

#endif //PIALARM_CURRENTINDOORSERVICE_H