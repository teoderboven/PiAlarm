#include <mutex>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <chrono>

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/pattern_formatter.h>

#include "logging/HasLogger.h"

namespace PiAlarm::logging {

    namespace { // Anonymous namespace for internal linkage

        /**
         * @brief Parses a string to determine the corresponding spdlog log level.
         *
         * This function takes a string representation of a log level and returns the corresponding spdlog::level::level_enum.
         * If the string does not match any known log level, it defaults to spdlog::level::info.
         *
         * @param level A string representing the desired log level (e.g., "trace", "debug", "info", "warn", "error", "critical").
         * @return The corresponding spdlog log level.
         */
        constexpr spdlog::level::level_enum parseLogLevel(const char* level) {
            if (strcmp(level, "trace") == 0) return spdlog::level::trace;
            if (strcmp(level, "debug") == 0) return spdlog::level::debug;
            if (strcmp(level, "info") == 0) return spdlog::level::info;
            if (strcmp(level, "warn") == 0) return spdlog::level::warn;
            if (strcmp(level, "error") == 0) return spdlog::level::err;
            if (strcmp(level, "critical") == 0) return spdlog::level::critical;
            return spdlog::level::info; // default
        }

        /**
         * @brief Attempts to create a log directory and returns its path.
         *
         * This function tries to create a preferred log directory based on the operating system and build type.
         * If it fails, it falls back to a default log directory.
         *
         * @return The path to the log directory.
         */
        std::filesystem::path tryCreateLogPath() {
            namespace fs = std::filesystem;

        #ifdef _WIN32
            fs::path preferred = "log";
        #else // For Unix-like systems

            #ifdef BUILD_RELEASE
                fs::path preferred = "/var/log/mon_app";
            #else
                fs::path preferred = "log";
            #endif

        #endif

            fs::path fallback = "log";

            try {
                fs::create_directories(preferred);
                std::ofstream test(preferred / "test.log");
                if (!test) throw std::runtime_error("Not able to create log file in preferred path");
                test.close();
                fs::remove(preferred / "test.log");
                return preferred;
            } catch (...) {
                fs::create_directories(fallback);
                return fallback;
            }
        }

    } // unnamed namespace

    // HasLogger implementation ////////////////////////////////////////////////////////////////////////////////////////

    namespace { // Anonymous namespace for internal linkage
        std::once_flag logger_initialized;
        std::shared_ptr<spdlog::logger> global_logger;
    }

    HasLogger::HasLogger(const std::string& name) {
        init_global_logger();

        // Named logger using the global sink
        logger_ = global_logger->clone(name);
    }

    void HasLogger::init_global_logger() {
        std::call_once(logger_initialized,
            []() {
                const std::filesystem::path logDir = tryCreateLogPath();
                const std::filesystem::path logFilePath = logDir / "PiAlarm.log";

                auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                    logFilePath.string(), 1024 * 1024 * 5, 2 // 5 MB max, 2 files
                );
                sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v");

                global_logger = std::make_shared<spdlog::logger>("global", sink);
                global_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%n] %v");
                spdlog::register_logger(global_logger);
                spdlog::set_default_logger(global_logger);
                spdlog::set_level(parseLogLevel(LOG_LEVEL)); // use macro

            #ifdef BUILD_RELEASE
                spdlog::flush_every(std::chrono::minutes(5));
            #else
                spdlog::flush_on(spdlog::level::trace);
            #endif
            }
        );
    }

} // namespace PiAlarm::logging
