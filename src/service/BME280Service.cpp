#ifdef SENSOR_BME280

#include "service/BME280Service.h"

namespace PiAlarm::service {

    BME280Service::BME280Service(
        model::CurrentIndoorData& currentIndoorData
    )
        : BaseService("BME280Service"),
          currentIndoorData_{currentIndoorData}
    {}

    bool BME280Service::onStart() {
        try {
            BME280_.initialize();
            measurementDelay_ = BME280_.getMeasurementDelay();

            return true;
        } catch (const std::exception& e) {
            logger().error("Failed to start BME280 sensor: {}", e.what());
            return false;
        }
    }

    void BME280Service::update() {
        using hardware::BME280;
        try {
            BME280_.setMode(BME280::Mode::Forced);
            std::this_thread::sleep_for(measurementDelay_);
            const auto measurement = BME280_.readMeasurement();

            const auto coorectedTemperature = measurement.temperature + BME280_TEMPERATURE_OFFSET;

            currentIndoorData_.setValues(coorectedTemperature, measurement.humidity, measurement.pressure);

            logger().debug("BME280 data: {}°C (+{}), {}%, {}hPa", coorectedTemperature, BME280_TEMPERATURE_OFFSET, measurement.humidity, measurement.pressure);

        } catch (const std::exception& e) {
            logger().error("Failed to read from BME280 sensor: {}", e.what());
        }
    }

    std::chrono::milliseconds BME280Service::updateInterval() const {
        return std::chrono::seconds(30);
    }

} // namespace PiAlarm::service

#endif // SENSOR_BME280
