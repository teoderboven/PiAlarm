#ifdef RASPBERRY_PI

#include "hardware/BME280.h"

#include <bits/this_thread_sleep.h>

namespace PiAlarm ::hardware {

    BME280::BME280(I2C& i2c)
        : i2c_{i2c} {
        initialize();
    }

    void BME280::reset() const {
        i2c_.writeRegister(BME280_REG_RESET, BME280_RESET_COMMAND);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void BME280::setMode(Mode mode) const {
        uint8_t ctrl_meas;
        i2c_.readRegister(BME280_REG_CTRL_MEAS, &ctrl_meas, 1); // read current reg value
        ctrl_meas = (ctrl_meas & 0b11111100) | static_cast<uint8_t>(mode);
        i2c_.writeRegister(BME280_REG_CTRL_MEAS, ctrl_meas); // rewrite with new mode
    }

    void BME280::initialize() {
        reset();
        readCalibrationData();
        setOversampling(Oversampling::x1, Oversampling::x1, Oversampling::x1);
        setMode(Mode::Normal);
    }

    void BME280::setOversampling(Oversampling temp, Oversampling press, Oversampling hum) const {
        i2c_.writeRegister(BME280_REG_CTRL_HUM, static_cast<uint8_t>(hum));

        uint8_t ctrl_meas =
            (static_cast<uint8_t>(temp)  << 5) |
            (static_cast<uint8_t>(press) << 2) |
            static_cast<uint8_t>(Mode::Sleep);

        i2c_.writeRegister(BME280_REG_CTRL_MEAS, ctrl_meas);
    }

    BME280::Measurement BME280::readMeasurement() {
        uint8_t data[BME280_DATA_LENGTH];
        i2c_.readRegister(BME280_DATA_START, data, BME280_DATA_LENGTH);

        int32_t adc_P = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
        int32_t adc_T = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
        int32_t adc_H = (data[6] << 8)  | data[7];

        float temperature = compensateTemperature(adc_T) / 100.0f;
        float pressure = compensatePressure(adc_P) / 100.0f;
        float humidity  = compensateHumidity(adc_H) / 1024.0f;

        return { temperature, humidity, pressure };
    }

    void BME280::readCalibrationData() {
        uint8_t buf[BME280_CALIB_DATA_LENGTH];
        i2c_.readRegister(BME280_CALIB_DATA_START, buf, BME280_CALIB_DATA_LENGTH);

        calibration_.dig_T1 = buf[1] << 8 | buf[0];
        calibration_.dig_T2 = buf[3] << 8 | buf[2];
        calibration_.dig_T3 = buf[5] << 8 | buf[4];

        calibration_.dig_P1 = buf[7] << 8 | buf[6];
        calibration_.dig_P2 = buf[9] << 8 | buf[8];
        calibration_.dig_P3 = buf[11] << 8 | buf[10];
        calibration_.dig_P4 = buf[13] << 8 | buf[12];
        calibration_.dig_P5 = buf[15] << 8 | buf[14];
        calibration_.dig_P6 = buf[17] << 8 | buf[16];
        calibration_.dig_P7 = buf[19] << 8 | buf[18];
        calibration_.dig_P8 = buf[21] << 8 | buf[20];
        calibration_.dig_P9 = buf[23] << 8 | buf[22];

        calibration_.dig_H1 = buf[25];

        uint8_t hbuf[BME280_CALIB_DATA_HUM_LENGTH];
        i2c_.readRegister(BME280_CALIB_DATA_HUM_START, hbuf, BME280_CALIB_DATA_HUM_LENGTH);

        calibration_.dig_H2 = hbuf[1] << 8 | hbuf[0];
        calibration_.dig_H3 = hbuf[2];
        calibration_.dig_H4 = (hbuf[3] << 4) | (hbuf[4] & 0x0F);
        calibration_.dig_H5 = (hbuf[5] << 4) | (hbuf[4] >> 4);
        calibration_.dig_H6 = static_cast<int8_t>(hbuf[6]);
    }

    // Compensation formulas from BME280 datasheet (Section 4.2.3)
    // These functions convert raw ADC values to compensated values
    // of temperature (in °C * 100), pressure (in Pa), and humidity (in %RH * 1024)
    // using the calibration data read from the sensor.
    // The t_fine_ variable is used as an intermediate value in the compensation calculations.
    // See the datasheet for detailed explanation of the algorithms.
    // Made with the help of ChatGPT

    uint32_t BME280::compensatePressure(int32_t adc_P) const {
        int64_t var1, var2;

        var1 = static_cast<int64_t>(t_fine_) - 128000;
        var2 = var1 * var1 * static_cast<int64_t>(calibration_.dig_P6);
        var2 = var2 + ((var1 * static_cast<int64_t>(calibration_.dig_P5)) << 17);
        var2 = var2 + (static_cast<int64_t>(calibration_.dig_P4) << 35);
        var1 = ((var1 * var1 * static_cast<int64_t>(calibration_.dig_P3)) >> 8) +
               ((var1 * static_cast<int64_t>(calibration_.dig_P2)) << 12);
        var1 = (((static_cast<int64_t>(1) << 47) + var1)) *
               static_cast<int64_t>(calibration_.dig_P1) >> 33;

        if (var1 == 0) {
            return 0; // avoid divide by 0
        }

        int64_t p = 1048576 - adc_P;
        p = (((p << 31) - var2) * 3125) / var1;
        var1 = (static_cast<int64_t>(calibration_.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
        var2 = (static_cast<int64_t>(calibration_.dig_P8) * p) >> 19;

        p = ((p + var1 + var2) >> 8) +
            (static_cast<int64_t>(calibration_.dig_P7) << 4);

        return static_cast<uint32_t>(p);
    }

    int32_t BME280::compensateTemperature(int32_t adc_T) {
        int32_t var1, var2;

        var1 = (((adc_T >> 3) - (static_cast<int32_t>(calibration_.dig_T1) << 1)) *
                static_cast<int32_t>(calibration_.dig_T2)) >> 11;

        var2 = (((((adc_T >> 4) - static_cast<int32_t>(calibration_.dig_T1)) *
                  ((adc_T >> 4) - static_cast<int32_t>(calibration_.dig_T1))) >> 12) *
                static_cast<int32_t>(calibration_.dig_T3)) >> 14;

        t_fine_ = var1 + var2;

        return (t_fine_ * 5 + 128) >> 8;
    }

    uint32_t BME280::compensateHumidity(int32_t adc_H) const {
        int32_t v_x1_u32r = t_fine_ - 76800;
        v_x1_u32r = (
                        ((adc_H << 14) -
                            (static_cast<int32_t>(calibration_.dig_H4) << 20) -
                            static_cast<int32_t>(calibration_.dig_H5) * v_x1_u32r +
                            16384
                        ) >> 15) *
                        (((((((v_x1_u32r *
                            static_cast<int32_t>(calibration_.dig_H6)) >> 10) *
                          (((v_x1_u32r *
                             static_cast<int32_t>(calibration_.dig_H3)) >> 11) +
                           32768)) >> 10) +
                        2097152) *
                        static_cast<int32_t>(calibration_.dig_H2) + 8192) >> 14);

        v_x1_u32r = v_x1_u32r -
            (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
              static_cast<int32_t>(calibration_.dig_H1)) >> 4);

        if (v_x1_u32r < 0)
            v_x1_u32r = 0;
        if (v_x1_u32r > 419430400)
            v_x1_u32r = 419430400;

        return static_cast<uint32_t>(v_x1_u32r >> 12);
    }

} // namespace PiAlarm::hardware

#endif // RASPBERRY_PI
