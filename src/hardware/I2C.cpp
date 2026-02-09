#ifdef RASPBERRY_PI

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdexcept>
#include <cstring>
#include <cerrno>

#include "hardware/I2C.h"

namespace PiAlarm::hardware {

    I2C::I2C(const std::string& device, uint8_t address)
        : fd_{open(device.c_str(), O_RDWR)}, addr_{address}
    {
        if (fd_ < 0) {
            throw std::runtime_error("Unable to open I2C device: " + device + " : " + std::strerror(errno));
        }

        // Set the I2C slave address
        if (ioctl(fd_, I2C_SLAVE, addr_) < 0) {
            close(fd_);
            throw std::runtime_error("Unable to select I2C slave 0x" + std::to_string(addr_) + " : " + std::strerror(errno));
        }
    }

    I2C::~I2C() {
        if (fd_ >= 0) {
            close(fd_);
        }
    }

    void I2C::writeData(const uint8_t* data, size_t length) const {
        const ssize_t written = write(fd_, data, length);
        if (written != static_cast<ssize_t>(length)) {
            throw std::runtime_error("I2C write failed: " + std::string(std::strerror(errno)));
        }
    }

    void I2C::readData(uint8_t* buffer, size_t length) const {
        const ssize_t readBytes = read(fd_, buffer, length);
        if (readBytes != static_cast<ssize_t>(length)) {
            throw std::runtime_error("I2C read failed: " + std::string(std::strerror(errno)));
        }
    }

    void I2C::writeRegister(uint8_t reg, uint8_t value) const {
        uint8_t buffer[2] = { reg, value };
        writeData(buffer, sizeof(buffer));
    }

    void I2C::readRegister(uint8_t reg, uint8_t* buffer, size_t length) const {
        // Write register address
        if (write(fd_, &reg, 1) != 1) {
            throw std::runtime_error("I2C write(register) failed: " + std::string(std::strerror(errno)));
        }

        readData(buffer, length);
    }

} // namespace PiAlarm::hardware

#endif // RASPBERRY_PI
