#ifndef LOGUTILS_H
#define LOGUTILS_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <cstring>
#include <chrono>
#include <filesystem>
#include <fstream>

/**
 * @file LogUtils.h
 * @brief Utility functions for configuring logging in the PiAlarm application.
 *
 * This header file provides functions to parse log levels from strings and configure the spdlog logger.
 */

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
inline std::filesystem::path try_create_log_path() {
    namespace fs = std::filesystem;

#ifdef _WIN32
    fs::path preferred = "log";
#else
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

/**
 * @brief Configures the logging system for the PiAlarm application.
 *
 * This function initializes the spdlog logger with a basic file sink and sets the log level based on the provided string.
 * The log messages will be written to "logs/PiAlarm.log" with a specific format.
 *
 * @param level A string representing the desired log level (e.g., "trace", "debug", "info", "warn", "error", "critical").
 */
inline void configureLogging(const char* level) {
    const std::filesystem::path log_dir = try_create_log_path();
    const std::filesystem::path log_file = log_dir / "PiAlarm.log";

    const auto logger = spdlog::rotating_logger_mt("PiAlarm_logger", log_file.string(), 1048576 * 5, 2); // 5 MB max size, 2 files

    spdlog::set_default_logger(logger);
    spdlog::set_level(parseLogLevel(level));
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
    

#ifdef BUILD_RELEASE
    spdlog::flush_every(std::chrono::minutes(5));
#else
    spdlog::flush_on(spdlog::level::trace);
#endif
}

#endif //LOGUTILS_H
