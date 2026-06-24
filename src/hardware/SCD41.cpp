#ifdef RASPBERRY_PI

#include <unistd.h>
#include <stdexcept>
#include <thread>

#include "hardware/SCD41.h"

namespace PiAlarm::hardware {

    SCD41::SCD41(uint8_t address)
        : i2c_{address} {
    }

    void SCD41::sendCommand(uint16_t command) const {
        uint8_t buffer[3];

        // command byte
        buffer[0] = static_cast<uint8_t>(command >> 8);
        buffer[1] = static_cast<uint8_t>(command & 0xFF);

        // crc
        buffer[2] = computeCRC(buffer, 2);

        i2c_.writeData(buffer, 3);
    }

    void SCD41::sendCommand(uint16_t command, uint16_t data) {
        uint8_t buffer[5];

        // command bytes
        buffer[0] = static_cast<uint8_t>(command >> 8);
        buffer[1] = static_cast<uint8_t>(command & 0xFF);

        // data bytes
        buffer[2] = static_cast<uint8_t>(data >> 8);
        buffer[3] = static_cast<uint8_t>(data & 0xFF);

        // crc computed only over the data bytes
        buffer[4] = computeCRC(&buffer[2], 2);

        i2c_.writeData(buffer, 5);
    }

    void SCD41::readResponse(uint8_t* buffer, size_t length) const {
        i2c_.readData(buffer, length);
    }

    void SCD41::startPeriodicMeasurement() {
        sendCommand(SCD41_START_PERIODIC_MEASUREMENT);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    void SCD41::startLowPowerPeriodicMeasurement() {
        sendCommand(SCD41_START_LOW_POWER_PERIODIC_MEASUREMENT);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    void SCD41::stopPeriodicMeasurement() {
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

    SCD41::Measurement SCD41::readMeasurement() const {
        sendCommand(SCD41_READ_MEASUREMENT);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        uint8_t buffer[9];
        readResponse(buffer, sizeof(buffer));

        for (int i = 0; i < 3; i++) {
            checkCRC(&buffer[i * 3], 2, buffer[i * 3 + 2]);
        }

        const uint16_t co2_raw = (buffer[0] << 8) | buffer[1];
        const uint16_t temperature_raw = (buffer[3] << 8) | buffer[4];
        const uint16_t humidity_raw = (buffer[6] << 8) | buffer[7];

        return Measurement {
            .co2 = co2_raw,
            .temperature = convertTemperature(temperature_raw),
            .humidity = convertHumidity(humidity_raw)
        };
    }

    void SCD41::setTemperatureOffset(float offset_celsius) {
        const uint16_t data = convertTemperatureOffset(offset_celsius);
        sendCommand(SCD41_SET_TEMPERATURE_OFFSET, data);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    float SCD41::getTemperatureOffset() const {
        sendCommand(SCD41_GET_TEMPERATURE_OFFSET);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        uint8_t buffer[3];
        readResponse(buffer, sizeof(buffer));

        checkCRC(buffer, 2, buffer[2]);

        const uint16_t offset_raw = (buffer[0] << 8) | buffer[1];
        return convertRawToTemperatureOffset(offset_raw);
    }

    void SCD41::setAmbientPressure(uint16_t pressure_hpa) {
        sendCommand(SCD41_SET_AMBIENT_PRESSURE, pressure_hpa);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
