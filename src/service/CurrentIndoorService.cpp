#ifdef SENSOR_SCD41

#include "service/CurrentIndoorService.h"

namespace PiAlarm::service {

    CurrentIndoorService::CurrentIndoorService(
        model::CurrentIndoorData& currentIndoorData
    )
        : BaseService("CurrentIndoorService"),
          currentIndoorData_{currentIndoorData}
    {}

    bool CurrentIndoorService::onStart() {
        try {
            scd41_.stopPeriodicMeasurement(); // Ensure the sensor is stopped before starting a new measurement mode

            scd41_.setTemperatureOffset(0.8);
            scd41_.startLowPowerPeriodicMeasurement(); // 30s response time

            return true;
        } catch (const std::exception& e) {
            logger().error("Failed to start SCD41 sensor: {}", e.what());
            return false;
        }
    }

    void CurrentIndoorService::onStop() {
        try {
            scd41_.stopPeriodicMeasurement();
        } catch (const std::exception& e) {
            logger().error("Failed to stop SCD41 sensor: {}", e.what());
        }
    }

    void CurrentIndoorService::update() {
        try {
            if (scd41_.dataReady()) {
                const auto measurement = scd41_.readMeasurement();
                currentIndoorData_.setValues(measurement.temperature, measurement.humidity);

                logger().debug("SCD41 data: {}°C, {}%, {}ppm", measurement.temperature, measurement.humidity, measurement.co2);
            }
        } catch (const std::exception& e) {
            logger().error("Failed to read from SCD41 sensor: {}", e.what());
        }
    }

    std::chrono::milliseconds CurrentIndoorService::updateInterval() const {
        return std::chrono::seconds(30);
    }

} // namespace PiAlarm::service

#endif // SENSOR_SCD41
