#include "utils/consoleUtils.hpp"
#include "Application.h"

/**
 * @brief Main function for the PiAlarm application.
 *
 * This function initializes the application, sets up the console if DISPLAY_CONSOLE is defined,
 * and starts the main application loop. It also handles input events for button presses on Linux systems.
 *
 * @param argc unused.
 * @param argv unused.
 * @return Exit code (never returns)
 */
[[noreturn]]
int main(int argc, char *argv[]) {
    using namespace PiAlarm;

#ifdef DISPLAY_CONSOLE
    utils::setup_console();
#endif

    Application app{};
    app.run();
}
