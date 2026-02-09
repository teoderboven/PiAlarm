#ifndef PIALARM_BME280_H
#define PIALARM_BME280_H

#ifdef RASPBERRY_PI

#include <cstdint>

#include "hardware/I2C.h"

// BME280 register and data definitions
// https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf section 5.3
#define BME280_CALIB_DATA_START 0x88
#define BME280_CALIB_DATA_LENGTH 26
#define BME280_CALIB_DATA_HUM_START 0xE1
#define BME280_CALIB_DATA_HUM_LENGTH 7
#define BME280_REG_RESET 0xE0
#define BME280_RESET_COMMAND 0xB6
#define BME280_REG_CTRL_HUM 0xF2
#define BME280_REG_CTRL_MEAS 0xF4
#define BME280_REG_CONFIG 0xF5
#define BME280_DATA_START 0xF7 // Pressure MSB register
#define BME280_DATA_LENGTH 8 // Pressure(3) + Temperature(3) + Humidity(2)

namespace PiAlarm::hardware {

    /**
     * @class BME280
     * @brief Driver for the BME280 temperature, pressure, and humidity sensor.
     *
     * This class provides methods to initialize the sensor, configure its settings,
     * and read compensated measurement data.
     */
    class BME280 {
        const I2C& i2c_; ///< Reference to the I2C interface used for communication

    public:
        static constexpr uint8_t I2C_ADDRESS = 0x76; ///< I2C address of the BME280 sensor

        /**
         * @brief Operating modes for the BME280 sensor.
         */
        enum class Mode : uint8_t {
            Sleep = 0b00, ///< Sleep mode (no measurement)
            Forced = 0b01, ///< Forced mode (one-time measurement)
            Normal = 0b11 ///< Normal mode (continuous measurement)
        };

        /**
         * @brief Oversampling settings for the BME280 sensor.
         */
        enum class Oversampling : uint8_t {
            Skipped = 0, ///< No measurement
            x1 = 1, ///< Oversampling x1
            x2 = 2, ///< Oversampling x2
            x4 = 3, ///< Oversampling x4
            x8 = 4, ///< Oversampling x8
            x16 = 5 ///< Oversampling x16
        };

        /**
         * @brief Structure to hold measurement data from the BME280 sensor.
         */
        struct Measurement {
            float temperature; ///< Temperature in °C
            float humidity; ///< Relative humidity in %
            float pressure; ///< Pressure in hPa
        };

        /**
         * @brief Constructs a BME280 object with the specified I2C interface.
         * @param i2c Reference to the I2C interface used for communication.
         */
        explicit BME280(I2C& i2c);

        /**
         * @brief Resets the BME280 sensor.
         */
        void reset() const;

        /**
         * @brief Initializes the BME280 sensor with default settings.
         * The sensor is reset, calibration data is read, oversampling is set to x1,
         * and the sensor is set to Normal mode.
         */
        void initialize();

        /**
         * @brief Sets the operating mode of the BME280 sensor.
         * @param mode The desired operating mode.
         * @see Mode the enum for available modes.
         */
        void setMode(Mode mode) const;

        /**
         * @brief Sets the oversampling settings for temperature, pressure, and humidity.
         * @param temp Oversampling setting for temperature.
         * @param press Oversampling setting for pressure.
         * @param hum Oversampling setting for humidity.
         * @note The sensor must be in Sleep mode when changing oversampling settings.
         */
        void setOversampling(Oversampling temp, Oversampling press, Oversampling hum) const;

        /**
         * @brief Reads the latest measurement data from the BME280 sensor.
         * @return Measurement structure containing temperature, humidity, and pressure data.
         */
        Measurement readMeasurement();

    private:

        /**
         * @brief Calibration data structure for the BME280 sensor.
         * See BME280 datasheet section 4.2.2 for details.
         */
        struct Calibration {
            uint16_t dig_T1;
            int16_t  dig_T2, dig_T3;
            uint16_t dig_P1;
            int16_t  dig_P2, dig_P3, dig_P4, dig_P5,
                     dig_P6, dig_P7, dig_P8, dig_P9;
            uint8_t  dig_H1;
            int16_t  dig_H2;
            uint8_t  dig_H3;
            int16_t  dig_H4, dig_H5;
            int8_t   dig_H6;
        };

        Calibration calibration_; ///< Calibration data of the sensor
        int32_t t_fine_ = 0; ///< Fine temperature value for compensation calculations

        /**
         * @brief Reads the calibration data from the BME280 sensor.
         * This data is used for compensating raw sensor readings.
         * Sets the calibration_ member variable.
         */
        void readCalibrationData();

        // Compensation functions based on BME280 datasheet algorithms
        /**
         * @brief Compensates the raw pressure reading.
         * @param adc_P Raw pressure reading from the sensor.
         * @return Compensated pressure in Pa.
         */
        uint32_t compensatePressure(int32_t adc_P) const;

        /**
         * @brief Compensates the raw temperature reading.
         * @param adc_T Raw temperature reading from the sensor.
         * @return Compensated temperature in °C as an integer (scaled by 100).
         */
        int32_t compensateTemperature(int32_t adc_T);

        /**
         * @brief Compensates the raw humidity reading.
         * @param adc_H Raw humidity reading from the sensor.
         * @return Compensated relative humidity in % (scaled by 1024).
         */
        uint32_t compensateHumidity(int32_t adc_H) const;

    };

} // namespace PiAlarm::hardware

#endif // RASPBERRY_PI
#endif //PIALARM_BME280_H