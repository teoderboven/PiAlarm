#ifndef PIALARM_SCD41_H
#define PIALARM_SCD41_H

#ifdef RASPBERRY_PI

#include "hardware/I2C.h"

#include <cstdint>

// SCD41 Command Definitions
// from https://download.mikroe.com/documents/datasheets/SCD41%20Datasheet.pdf section 3.4
#define SCD41_START_PERIODIC_MEASUREMENT 0x21B1
#define SCD41_STOP_PERIODIC_MEASUREMENT 0x3F86
#define SCD41_GET_DATA_READY_STATUS 0xE4B8
#define SCD41_READ_MEASUREMENT 0xEC05

namespace PiAlarm::hardware {

    /**
     * @brief Class representing the SCD41 CO2, temperature, and humidity sensor.
     *
     * This class provides an interface to interact with the SCD41 sensor over I2C.
     * It allows for reading CO2 levels, temperature, and humidity data.
     */
    class SCD41 {
        const I2C& i2c_; ///< Reference to the I2C interface used for communication

    public:
        static constexpr uint8_t I2C_ADDRESS = 0x62; ///< I2C address of the SCD41 sensor

        /**
         * @brief Structure to hold measurement data from the SCD41 sensor.
         */
        struct Measurement {
            uint16_t co2; ///< CO2 concentration in ppm
            float temperature; ///< Temperature in °C
            float humidity; ///< Relative humidity in %
        };

        /**
         * @brief Constructs an SCD41 object with the specified I2C interface.
         * @param i2c Reference to the I2C interface used for communication.
         */
        explicit SCD41(I2C& i2c);

        /**
         * @brief Starts periodic measurement mode on the SCD41 sensor.
         */
        void startPeriodicMeasurement() const;

        /**
         * @brief Stops periodic measurement mode on the SCD41 sensor.
         */
        void stopPeriodicMeasurement() const;

        /**
         * @brief Checks if new measurement data is available.
         * @return true if data is ready, false otherwise.
         */
        bool dataReady() const;

        /**
         * @brief Reads the latest measurement data from the SCD41 sensor.
         * @return Measurement structure containing CO2, temperature, and humidity data.
         */
        Measurement readMeasurement();

    private:

        /**
         * @brief Sends a command to the SCD41 sensor.
         * @param command 16-bit command to send.
         */
        void sendCommand(uint16_t command) const;

        /**
         * @brief Reads a response from the SCD41 sensor.
         * @param buffer Buffer to store the response data.
         * @param length Number of bytes to read.
         */
        void readResponse(uint8_t* buffer, size_t length) const;

        /**
         * @brief Converts raw temperature value to degrees Celsius.
         * @param raw Raw temperature value from the sensor.
         * @return Temperature in degrees Celsius.
         */
        static inline float convertTemperature(uint16_t raw) {
            return -45.0f + 175.0f * (static_cast<float>(raw) / 65535.0f); // from datasheet section 3.5.2
        }

        /**
         * @brief Converts raw humidity value to percentage.
         * @param raw Raw humidity value from the sensor.
         * @return Humidity in percentage (0.0 to 100.0).
         */
        static inline float convertHumidity(uint16_t raw) {
            return 100.0f * (static_cast<float>(raw) / 65535.0f); // from datasheet section 3.5.2
        }

        /**
         * @brief Computes the CRC8 checksum for the given data.
         * @param data Pointer to the data bytes.
         * @param length Number of bytes to compute the CRC for.
         * @return Computed CRC8 checksum.
         */
        static uint8_t computeCRC(const uint8_t* data, size_t length);

        /**
         * @brief Checks the CRC8 checksum of the given data against the provided CRC.
         * @param data Pointer to the data bytes.
         * @param length Number of bytes to check.
         * @param crc Expected CRC8 checksum.
         * @throws std::runtime_error if the CRC does not match.
         */
        static void checkCRC(const uint8_t* data, size_t length, uint8_t crc);

    };

} // namespace PiAlarm::hardware

#endif // RASPBERRY_PI
#endif //PIALARM_SCD41_H