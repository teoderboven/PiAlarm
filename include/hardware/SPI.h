#ifndef SPI_H
#define SPI_H

#include <string>

namespace PiAlarm::hardware {

    /**
     * @class SPI
     * @brief Represents a Serial Peripheral Interface (SPI) device.
     *
     * This class provides methods to interact with an SPI device,
     * allowing for reading and writing data over the SPI bus.
     *
     * SPI (Serial Peripheral Interface) is a synchronous serial communication protocol
     * used for short-distance communication, primarily in embedded systems.
     */
    class SPI {
        int fd_; ///< File descriptor for the SPI device

    public:

        /**
         * Constructs an SPI object for a specific device.
         * @param device The path to the SPI device (e.g., "/dev/spidev0.0").
         * @param speed The speed of the SPI communication in Hz (default is 10 MHz).
         * @throws std::runtime_error if the SPI device cannot be opened or configured.
         */
        explicit SPI(const std::string& device, uint32_t speed = 10'000'000);

        /**
         * Destructor for the SPI class.
         * Closes the SPI device file descriptor.
         */
        ~SPI();

        SPI(const SPI&) = delete; ///< No copy constructor
        SPI& operator=(const SPI&) = delete; ///< No copy assignment operator
        SPI(SPI&&) = delete; ///< No move constructor
        SPI& operator=(SPI&&) = delete; ///< No move assignment operator

        /**
         * Reads a single byte from the SPI device.
         * @return The byte read from the device.
         * @throws std::runtime_error if the read operation fails.
         */
        void writeByte(uint8_t byte);

        /**
         * Reads a specified number of bytes from the SPI device.
         * @param length The number of bytes to read.
         * @return A pointer to the buffer containing the read bytes.
         * @throws std::runtime_error if the read operation fails.
         */
        void writeData(const uint8_t* data, size_t length);

    };

} // namespace PiAlarm::hardware

#endif //SPI_H
