#include <stdexcept>
#include <cerrno>
#include <cstring>
#include "hardware/GPIO.h"

namespace PiAlarm::hardware {

    GPIO::GPIO(const std::string &chipName, unsigned int lineNumber) {
        chip_ = gpiod_chip_open_by_name(chipName.c_str());
        if (!chip_) {
            throw std::runtime_error("Unable to open GPIO chip: " + chipName + " : " + std::strerror(errno));
        }
        line_ = gpiod_chip_get_line(chip_, lineNumber);
        if (!line_) {
            gpiod_chip_close(chip_);
            throw std::runtime_error("Unable to get GPIO line " + std::to_string(lineNumber) + " from chip: " + chipName + " : " + std::strerror(errno));
        }
    }

    GPIO::GPIO(unsigned int lineNumber) : GPIO{"gpiochip0", lineNumber}
    {}

    GPIO::~GPIO() {
        if (line_) gpiod_line_release(line_);
        if (chip_) gpiod_chip_close(chip_);
    }

    void GPIO::setOutput(int initialValue) {
        if (gpiod_line_request_output(line_, "gpio_cpp", initialValue) < 0) {
            throw std::runtime_error("Unable to set GPIO line as output : " + std::string(std::strerror(errno)));
        }
    }

    void GPIO::setInput() {
        if (gpiod_line_request_input(line_, "gpio_cpp") < 0) {
            throw std::runtime_error("Unable to set GPIO line as input : " + std::string(std::strerror(errno)));
        }
    }

    void GPIO::set(int value) {
        if (gpiod_line_set_value(line_, value) < 0) {
            throw std::runtime_error("Unable to set GPIO line value : " + std::string(std::strerror(errno)));
        }
    }

    int GPIO::get() {
        int val = gpiod_line_get_value(line_);
        if (val < 0) {
            throw std::runtime_error("Unable to read GPIO line value : " + std::string(std::strerror(errno)));
        }
        return val;
    }

} // namespace PiAlarm::hardware
