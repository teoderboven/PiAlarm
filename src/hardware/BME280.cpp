#ifdef RASPBERRY_PI

#include "hardware/BME280.h"

#include <thread>
#include <cmath>

namespace PiAlarm ::hardware {

    BME280::BME280(uint8_t address)
        : i2c_{address}
    {}

    void BME280::reset() {
        i2c_.writeRegister(REG_RESET, RESET_COMMAND);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void BME280::setMode(Mode mode) {
        uint8_t ctrl_meas;
        i2c_.readRegister(REG_CTRL_MEAS, &ctrl_meas, 1); // read current reg value
        ctrl_meas = (ctrl_meas & 0b11111100) | static_cast<uint8_t>(mode);
        i2c_.writeRegister(REG_CTRL_MEAS, ctrl_meas); // rewrite with new mode
    }

    void BME280::initialize() {
        reset();
        readCalibrationData();
        setOversampling(Oversampling::x2, Oversampling::x16, Oversampling::x1);
    }

    void BME280::setOversampling(Oversampling temp, Oversampling press, Oversampling hum) {
        i2c_.writeRegister(REG_CTRL_HUM, static_cast<uint8_t>(hum));

        uint8_t current_ctrl;
        i2c_.readRegister(REG_CTRL_MEAS, &current_ctrl, 1);
        uint8_t current_mode = current_ctrl & 0b00000011;

        uint8_t ctrl_meas =
            (static_cast<uint8_t>(temp)  << 5) |
            (static_cast<uint8_t>(press) << 2) |
            current_mode;

        i2c_.writeRegister(REG_CTRL_MEAS, ctrl_meas);

        currentTempOversampling_ = temp;
        currentPressOversampling_ = press;
        currentHumOversampling_ = hum;
    }

    BME280::Measurement BME280::readMeasurement() const {
        uint8_t data[DATA_LENGTH];
        i2c_.readRegister(DATA_START, data, DATA_LENGTH);

        int32_t adc_P = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
        int32_t adc_T = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
        int32_t adc_H = (data[6] << 8)  | data[7];

        const float temperature = compensateTemperature(adc_T) / 100.0f;
        const float pressure = (compensatePressure(adc_P) >> 8) / 100.0f;
        const float humidity  = compensateHumidity(adc_H) / 1024.0f;

        return { temperature, humidity, pressure };
    }

    std::chrono::milliseconds BME280::getMeasurementDelay() const {
        /// Converts the oversampling enum value to the actual oversampling factor used in the delay calculation.
        auto toRealValue = [](Oversampling os) -> int {
            auto const val = static_cast<uint8_t>(os);
            if (val == 0) return 0;
            return 1 << (val - 1); // 1 => 1, 2 => 2, 3 => 4, 4 => 8, 5 => 16
        };

        const int t_os = toRealValue(currentTempOversampling_);
        const int p_os = toRealValue(currentPressOversampling_);
        const int h_os = toRealValue(currentHumOversampling_);

        // formula from datasheet (section 9.1)
        double delay_ms = 1.25 + (2.3 * t_os);

        if (p_os > 0) {
            delay_ms += (2.3 * p_os) + 0.575;
        }
        if (h_os > 0) {
            delay_ms += (2.3 * h_os) + 0.575;
        }

        return std::chrono::milliseconds(static_cast<long long>(std::ceil(delay_ms)) + 3);
    }

    void BME280::readCalibrationData() {
        uint8_t buf[CALIB_DATA_LENGTH];
        i2c_.readRegister(CALIB_DATA_START, buf, CALIB_DATA_LENGTH);

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

        uint8_t hbuf[CALIB_DATA_HUM_LENGTH];
        i2c_.readRegister(CALIB_DATA_HUM_START, hbuf, CALIB_DATA_HUM_LENGTH);

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

    int32_t BME280::compensateTemperature(int32_t adc_T) const {
        int32_t var1, var2;

        var1 = (((adc_T >> 3) - (static_cast<int32_t>(calibration_.dig_T1) << 1)) *
                static_cast<int32_t>(calibration_.dig_T2)) >> 11;

        var2 = (((((adc_T >> 4) - static_cast<int32_t>(calibration_.dig_T1)) *
                  ((adc_T >> 4) - static_cast<int32_t>(calibration_.dig_T1))) >> 12) *
                static_cast<int32_t>(calibration_.dig_T3)) >> 14;

        t_fine_ = var1 + var2;

        return (t_fine_ * 5 + 128) >> 8;
    }

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
