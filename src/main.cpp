#include "utils/consoleUtils.hpp"
#include "utils/argsUtils.hpp"
#include "Application.h"

/**
 * @brief Main function for the PiAlarm application.
 *
 * This function initializes the application, sets up the console if DISPLAY_CONSOLE is defined,
 * and starts the main application loop. It also handles input events for button presses on Linux systems.
 *
 * @param argc unused.
 * @param argv unused.
 * @return Exit code (never returns except for help)
 */
// [[noreturn]]
int main(int argc, char *argv[]) {
    using namespace PiAlarm;

#ifdef DISPLAY_CONSOLE
    utils::setup_console();
#endif

    if (utils::hasHelpFlag(argc, argv)) {
        utils::printHelp(argv[0]);
        return 0;
    }

    auto alarmCount = utils::getAlarmCount(argc, argv, 3);
    auto weatherCityName = utils::getWeatherLocation(argc, argv, "Brussel-1");
    auto customMusicFolderPath = utils::getMusicFolderPath(argc, argv, "");

    Application app{
        static_cast<size_t>(alarmCount), // Number of alarms from command line arguments
        std::chrono::minutes(5), // Default snooze duration
        std::chrono::minutes(60), // Default ring duration
        weatherCityName, // Weather location from command line arguments
        customMusicFolderPath // Custom music folder path from command line arguments
    };
    app.run();
}
