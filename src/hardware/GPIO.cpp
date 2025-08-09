#ifdef __linux__

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

    GPIO::~GPIO() {
        if (line_) gpiod_line_release(line_);
        if (chip_) gpiod_chip_close(chip_);
    }

    void GPIO::setOutput(int initialValue) {
        if (mode_ == GPIOMode::OUTPUT) {
            // If already set as output, just set the value
            set(initialValue);
            return;
        }

        gpiod_line_release(line_); // Release any previous request
        if (gpiod_line_request_output(line_, CONSUMER, initialValue) < 0) {
            throw std::runtime_error("Unable to set GPIO line as output : " + std::string(std::strerror(errno)));
        }
        mode_ = GPIOMode::OUTPUT;
    }

    void GPIO::setInput() {
        if (mode_ == GPIOMode::INPUT) {
            return;
        }

        gpiod_line_release(line_); // Release any previous request
        if (gpiod_line_request_input(line_, CONSUMER) < 0) {
            throw std::runtime_error("Unable to set GPIO line as input : " + std::string(std::strerror(errno)));
        }
        mode_ = GPIOMode::INPUT;
    }

    void GPIO::setInputWithEdgeDetection(EdgeType edge) {
        gpiod_line_release(line_);

        int requestResult {-1};
        switch (edge) {
            case EdgeType::RISING:
                requestResult = gpiod_line_request_rising_edge_events(line_, CONSUMER);
                break;
            case EdgeType::FALLING:
                requestResult = gpiod_line_request_falling_edge_events(line_, CONSUMER);
                break;
            case EdgeType::BOTH:
                requestResult = gpiod_line_request_both_edges_events(line_, CONSUMER);
                break;
            default:
                throw std::invalid_argument("Invalid edge type for event detection");
        }

        if (requestResult < 0) {
            throw std::runtime_error("Unable to set GPIO edge detection : " + std::string(std::strerror(errno)));
        }

        mode_ = GPIOMode::INPUT;
    }

    void GPIO::set(int value) const {
        if (mode_ != GPIOMode::OUTPUT) {
            throw std::runtime_error("GPIO line is not set as output, cannot set value.");
        }
        if (gpiod_line_set_value(line_, value) < 0) {
            throw std::runtime_error("Unable to set GPIO line value : " + std::string(std::strerror(errno)));
        }
    }

    int GPIO::get() const {
        if (mode_ != GPIOMode::INPUT) {
            throw std::runtime_error("GPIO line is not set as input, cannot get value.");
        }
        int val = gpiod_line_get_value(line_);
        if (val < 0) {
            throw std::runtime_error("Unable to read GPIO line value : " + std::string(std::strerror(errno)));
        }
        return val;
    }

    bool GPIO::waitForEvent(int timeoutMs) const {
        timespec timeout{}; // POSIX struct
        timeout.tv_sec = timeoutMs / 1000;
        timeout.tv_nsec = (timeoutMs % 1000) * 1000000L;

        auto ret = gpiod_line_event_wait(line_, &timeout);
        if (ret < 0) {
            throw std::runtime_error("Error waiting for GPIO event: " + std::string(std::strerror(errno)));
        }
        return ret > 0;
    }

    GPIOEvent GPIO::readEvent() const {
        gpiod_line_event ev{};
        if (gpiod_line_event_read(line_, &ev) < 0) {
            throw std::runtime_error("Error reading GPIO event: " + std::string(std::strerror(errno)));
        }

        GPIOEvent result;
        result.type = (ev.event_type == GPIOD_LINE_EVENT_RISING_EDGE) ? GPIOEvent::Type::RisingEdge : GPIOEvent::Type::FallingEdge;

        result.timestamp = std::chrono::steady_clock::now();

        return result;
    }

} // namespace PiAlarm::hardware

#endif // __linux__
