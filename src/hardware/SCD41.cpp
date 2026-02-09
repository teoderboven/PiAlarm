#ifdef RASPBERRY_PI

#include <unistd.h>
#include <stdexcept>
#include <thread>

#include "hardware/SCD41.h"

namespace PiAlarm::hardware {

    SCD41::SCD41(I2C& i2c)
        : i2c_{i2c} {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        stopPeriodicMeasurement();
    }

    void SCD41::sendCommand(uint16_t command) const {
        uint8_t cmd[2] = {
            static_cast<uint8_t>(command >> 8),
            static_cast<uint8_t>(command & 0xFF)
        };

        uint8_t crc = computeCRC(cmd, 2);
        uint8_t buffer[3] = { cmd[0], cmd[1], crc };

        i2c_.writeData(buffer, 3);
    }


    void SCD41::readResponse(uint8_t* buffer, size_t length) const {
        i2c_.readData(buffer, length);
    }

    void SCD41::startPeriodicMeasurement() const {
        sendCommand(SCD41_START_PERIODIC_MEASUREMENT);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    void SCD41::stopPeriodicMeasurement() const {
        sendCommand(SCD41_STOP_PERIODIC_MEASUREMENT);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    bool SCD41::dataReady() const {
        sendCommand(SCD41_GET_DATA_READY_STATUS);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        uint8_t buffer[3];
        readResponse(buffer, sizeof(buffer));

        checkCRC(buffer, 2, buffer[2]);

        uint16_t status = (buffer[0] << 8) | buffer[1];
        return (status & 0x07FF) != 0;
    }

    SCD41::Measurement SCD41::readMeasurement() {
        sendCommand(SCD41_READ_MEASUREMENT);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        uint8_t buffer[9];
        readResponse(buffer, sizeof(buffer));

        for (int i = 0; i < 3; i++) {
            checkCRC(&buffer[i * 3], 2, buffer[i * 3 + 2]);
        }

        uint16_t co2_raw = (buffer[0] << 8) | buffer[1];
        uint16_t temperature_raw = (buffer[3] << 8) | buffer[4];
        uint16_t humidity_raw = (buffer[6] << 8) | buffer[7];

        Measurement measurement;
        measurement.co2 = co2_raw;
        measurement.temperature = convertTemperature(temperature_raw);
        measurement.humidity = convertHumidity(humidity_raw);

        return measurement;
    }

    uint8_t SCD41::computeCRC(const uint8_t* data, size_t length) {
        uint8_t crc = 0xFF;
        for (size_t i = 0; i < length; i++) {
            crc ^= data[i];
            for (int bit = 0; bit < 8; bit++) {
                crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : (crc << 1);
            }
        }
        return crc;
    }

    void SCD41::checkCRC(const uint8_t* data, size_t length, uint8_t crc) {
        if (computeCRC(data, length) != crc) {
            throw std::runtime_error("SCD41 CRC check failed");
        }
    }

} // namespace PiAlarm::hardware

#endif // RASPBERRY_PI
