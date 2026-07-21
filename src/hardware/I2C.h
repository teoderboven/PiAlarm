#pragma once

#ifdef RASPBERRY_PI

#include <string>
#include <cstdint>
#include <utility>

namespace PiAlarm::hardware {

    /**
     * @struct I2CConfig
     * @brief Configuration for an I2C device.
     */
    struct I2CConfig {
        std::string device; ///< Device Path to I2C bus.
        uint8_t address;    ///< I2C slave address.

        /**
         * @brief Constructs an I2CConfig object.
         * @param device The device path to the I2C bus.
         * @param address The I2C slave address.
         */
        I2CConfig(std::string  device, uint8_t address) : device{std::move(device)}, address{address} {}

        /**
         * @brief Constructs an I2CConfig object for a specific address on the default I2C bus ("/dev/i2c-1").
         * @param address The I2C slave address.
         */
        explicit I2CConfig(uint8_t address) : I2CConfig("/dev/i2c-1", address) {}
    };

    /**
     * @class I2C
     * @brief Represents an I2C device on a Linux system.
     *
     * This class provides low-level read/write access to an I2C slave
     * using the Linux i2c-dev interface.
     *
     * Communication is done using read() and write(), without SMBus helpers.
     */
    class I2C {
        int fd_; ///< File descriptor for the I2C bus
        uint8_t addr_; ///< I2C slave address

    public:

        /**
         * @brief Constructs an I2C object for a specific device and address.
         * @param device Path to I2C bus (e.g. "/dev/i2c-1")
         * @param address I2C slave address
         * @throws std::runtime_error if the device cannot be opened or the address set.
         */
        I2C(const std::string& device, uint8_t address);

        /**
         * @brief Constructs an I2C object for a specific address on the default I2C bus ("/dev/i2c-1").
         * @param address I2C slave address
         * @throws std::runtime_error if the device cannot be opened or the address set.
         */
        explicit I2C(uint8_t address) : I2C("/dev/i2c-1", address) {}

        /**
         * @brief Constructs an I2C object from an I2CConfig.
         * @param config The I2C configuration.
         */
        explicit I2C(const I2CConfig& config) : I2C(config.device, config.address) {}


        /**
         * @brief Destructor for the I2C class.
         * Closes the I2C device.
         */
        ~I2C();

        I2C(const I2C&) = delete; ///< No copy constructor
        I2C& operator=(const I2C&) = delete; ///< No copy assignment operator
        I2C(I2C&&) = delete; ///< No move constructor
        I2C& operator=(I2C&&) = delete; ///< No move assignment operator

        /**
         * @brief Writes raw data to the I2C slave.
         * @param data Pointer to the data to write.
         * @param length Number of bytes to write.
         */
        void writeData(const uint8_t* data, size_t length);

        /**
         * @brief Reads raw data from the I2C slave.
         * @param buffer Buffer to fill.
         * @param length Number of bytes to read.
         */
        void readData(uint8_t* buffer, size_t length) const;

        /**
         * @brief Writes a register address, then reads data from it.
         * @param reg Register address.
         * @param buffer Output buffer.
         * @param length Number of bytes to read.
         */
        void readRegister(uint8_t reg, uint8_t* buffer, size_t length) const;

        /**
         * @brief Writes a value to a register.
         * @param reg Register address.
         * @param value Value to write.
         */
        void writeRegister(uint8_t reg, uint8_t value);
    };

} // namespace PiAlarm::hardware

#endif // RASPBERRY_PI