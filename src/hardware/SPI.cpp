#ifdef RASPBERRY_PI

#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <cstring>
#include <cerrno>

#include "hardware/SPI.h"

namespace PiAlarm::hardware {

    SPI::SPI(const std::string& device, uint32_t speed)
        : fd_{open(device.c_str(), O_RDWR)}
    {
        if (fd_ < 0) {
            throw std::runtime_error("Unable to open SPI device: " + device + " : " + std::strerror(errno));
        }

        // Set the SPI mode
        /* SPI mode defines how data is synchronized with the clock signal (SCK).
           Here, SPI_MODE_0 means:
           - Clock polarity (CPOL) = 0: the clock line is low when idle.
           - Clock phase (CPHA) = 0: data is captured on the rising edge of the clock (first clock transition).
        */
        uint8_t mode = SPI_MODE_0;
        int ret = ioctl(fd_, SPI_IOC_WR_MODE, &mode);
        if (ret < 0) {
            close(fd_);
            throw std::runtime_error("Unable to set SPI mode: " + std::string(std::strerror(errno)));
        }

        // Sets the bits per word
        uint8_t bits = 8; // Default to 8 bits per word
        ret = ioctl(fd_, SPI_IOC_WR_BITS_PER_WORD, &bits);
        if (ret < 0) {
            close(fd_);
            throw std::runtime_error("Unable to set SPI bits per word: " + std::string(std::strerror(errno)));
        }

        // Set the SPI speed
        ret = ioctl(fd_, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
        if (ret < 0) {
            close(fd_);
            throw std::runtime_error("Unable to set SPI speed: " + std::string(std::strerror(errno)));
        }
    }

    SPI::~SPI() {
        if (fd_ >= 0) {
            close(fd_);
        }
    }

    void SPI::writeByte(uint8_t byte) const {
        writeData(&byte, sizeof(byte));
    }

    void SPI::writeData(const uint8_t* data, size_t length) const {
        size_t totalWritten = 0;

        while (totalWritten < length) {
            const size_t chunkSize = std::min(static_cast<size_t>(SPI_MAX_DATA_SIZE), length - totalWritten);
            const ssize_t bytesWritten = write(fd_, data + totalWritten, chunkSize);

            if (bytesWritten != static_cast<ssize_t>(chunkSize)) {
                throw std::runtime_error("SPI write failed: " + std::string(std::strerror(errno)));
            }

            totalWritten += bytesWritten;
        }
    }

} // namespace PiAlarm::hardware

#endif // RASPBERRY_PI
