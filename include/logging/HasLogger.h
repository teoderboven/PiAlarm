#ifndef HASLOGGER_H
#define HASLOGGER_H

#include <memory>
#include <string>
#include <spdlog/spdlog.h>

/**
 * @namespace PiAlarm::logging
 * @brief Namespace for logging utilities in the PiAlarm application.
 */
namespace PiAlarm::logging {

    /**
     * @class HasLogger
     * @brief Base class for components that require logging capabilities.
     *
     * This class provides a logger instance that can be used by derived classes to log messages.
     * It initializes the global logger on construction.
     */
    class HasLogger {
        // logger is mutable to allow access in const methods
        mutable std::shared_ptr<spdlog::logger> logger_; ///< The logger instance.

    protected:
        /**
         * @brief Constructs a HasLogger instance with a specified logger name.
         *
         * This constructor initializes the global logger and sets up a named logger for the derived class.
         *
         * @param name The name of the logger to be used by the derived class.
         */
        explicit HasLogger(const std::string& name);

    public:
        /**
         * @brief Default destructor.
         *
         * The destructor is virtual to allow proper cleanup of derived classes.
         */
        virtual ~HasLogger() = default;

    protected:
        /**
         * @brief Gives access to the logger instance.
         *
         * This method provides access to the logger instance for logging messages.
         *
         * @return A reference to the logger instance.
         */
        [[nodiscard]]
        inline spdlog::logger& logger() const {
            return *logger_;
        }

    private:
        /**
         * @brief Initializes the global logger.
         *
         * This method is called once to set up the global logger with a rotating file sink.
         * It creates a log directory if it does not exist and configures the logger with the specified log level.
         */
        static void init_global_logger();

    };

} // namespace PiAlarm::logging

#endif //HASLOGGER_H
