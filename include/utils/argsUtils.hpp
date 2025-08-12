#ifndef PIALARM_ARGSUTILS_HPP
#define PIALARM_ARGSUTILS_HPP

#include <iostream>
#include <string>
#include <cstdlib> // For std::atoi

namespace PiAlarm::utils {

    /**
     * @brief Checks if the help flag is present in the command line arguments.
     * @param argc The argument count.
     * @param argv The argument vector.
     * @return True if the help flag is present, false otherwise.
     */
    inline bool hasHelpFlag(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-h" || arg == "--help") {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Prints the help message for the command line arguments.
     * @param progName The name of the program to display in the usage message.
     */
    inline void printHelp(const std::string& progName) {
        std::cout << "Usage: " << progName << " [options]\n\n"
                  << "Options:\n"
                  << "  -a, --alarm-count <number>     Set the number of alarms (integer)\n"
                  << "  -l, --weaher-location <city>   Set the weather location (string)\n"
                  << "  -m, --music-dir <path>         Set the music folder path\n"
                  << "  -h, --help                     Show this help message\n";
    }

    /**
     * @brief Retrieves the number of alarms from command line arguments.
     * @param argc The argument count.
     * @param argv The argument vector.
     * @param defaultValue The default value to return if not specified.
     * @return The number of alarms specified or the default value.
     */
    inline int getAlarmCount(int argc, char* argv[], int defaultValue) {
        for (int i = 1; i < argc - 1; ++i) { // argc-1 to avoid overflow
            std::string arg = argv[i];
            if (arg == "-a" || arg == "--alarm-count") {
                return std::atoi(argv[i + 1]);
            }
        }
        return defaultValue;
    }

    /**
     * @brief Retrieves the weather location from command line arguments.
     * @param argc The argument count.
     * @param argv The argument vector.
     * @param defaultValue The default value to return if not specified.
     * @return The weather location specified or the default value.
     */
    inline std::string getWeatherLocation(int argc, char* argv[], const std::string& defaultValue) {
        for (int i = 1; i < argc - 1; ++i) {
            std::string arg = argv[i];
            if (arg == "-l" || arg == "--weather-location") {
                return argv[i + 1];
            }
        }
        return defaultValue;
    }

    /**
     * @brief Retrieves the music directory from command line arguments.
     * @param argc The argument count.
     * @param argv The argument vector.
     * @param defaultValue The default value to return if not specified.
     * @return The music directory specified or the default value.
     */
    inline std::string getMusicFolderPath(int argc, char* argv[], const std::string& defaultValue) {
        for (int i = 1; i < argc - 1; ++i) {
            std::string arg = argv[i];
            if (arg == "-m" || arg == "--music-dir") {
                return argv[i + 1];
            }
        }
        return defaultValue;
    }

} // namespace PiAlarm::utils

#endif //PIALARM_ARGSUTILS_HPP
