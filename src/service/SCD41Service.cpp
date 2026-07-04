#ifdef SENSOR_SCD41

#include "service/SCD41Service.h"

namespace PiAlarm::service {
    SCD41Service::SCD41Service(
        model::CO2Data& CO2Data,
        const model::CurrentIndoorData& currentIndoorData
    )
        : BaseService("SCD41Service"),
          co2Data_{CO2Data},
          currentIndoorData_{currentIndoorData}
    {}

    bool SCD41Service::onStart() {
        try {
            scd41_.stopPeriodicMeasurement(); // Ensure the sensor is stopped before starting a new measurement mode
            scd41_.startLowPowerPeriodicMeasurement(); // 30s response time

            return true;
        } catch (const std::exception& e) {
            logger().error("Failed to start SCD41 sensor: {}", e.what());
            return false;
        }
    }

    void SCD41Service::onStop() {
        try {
            scd41_.stopPeriodicMeasurement();
        } catch (const std::exception& e) {
            logger().error("Failed to stop SCD41 sensor: {}", e.what());
        }
    }

    void SCD41Service::process() {
        try {
            if (currentIndoorData_.isValid()) {
                // use indoor pressure to improve co2 measurement value
                scd41_.setAmbientPressure(static_cast<uint16_t>(currentIndoorData_.getPressure()));
            }

            if (scd41_.dataReady()) {
                const auto measurement = scd41_.readMeasurement();

                co2Data_.setValues(measurement.co2);

                logger().debug("SCD41 data: {}°C, {}%, {}ppm", measurement.temperature, measurement.humidity, measurement.co2);
            }
        } catch (const std::exception& e) {
            co2Data_.setValid(false);
            logger().error("Failed to read from SCD41 sensor: {}", e.what());
        }
    }

    std::chrono::milliseconds SCD41Service::updateInterval() const {
        return std::chrono::seconds(30);
    }

} // namespace PiAlarm::service

#endif // SENSOR_SCD41
