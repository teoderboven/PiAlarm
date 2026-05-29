#ifdef SENSOR_SCD41

#include "service/CurrentIndoorService.h"

namespace PiAlarm::service {

    CurrentIndoorService::CurrentIndoorService(
        model::CurrentIndoorData& currentIndoorData
    )
        : BaseService("CurrentIndoorService"),
          sensor{},
          currentIndoorData{currentIndoorData}
    {}

    bool CurrentIndoorService::onStart() {
        try {
            sensor.stopPeriodicMeasurement(); // Ensure the sensor is stopped before starting a new measurement mode
            sensor.startLowPowerPeriodicMeasurement(); // 30s response time
            return true;
        } catch (const std::exception& e) {
            logger().error("Failed to start SCD41 sensor: {}", e.what());
            return false;
        }
    }

    void CurrentIndoorService::onStop() {
        try {
            sensor.stopPeriodicMeasurement();
        } catch (const std::exception& e) {
            logger().error("Failed to stop SCD41 sensor: {}", e.what());
        }
    }

    void CurrentIndoorService::update() {
        try {
            if (sensor.dataReady()) {
                const auto measurement = sensor.readMeasurement();
                currentIndoorData.setValues(measurement.temperature, measurement.humidity);
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
