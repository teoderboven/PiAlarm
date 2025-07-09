#include <thread>

#include "display/DisplayConfig.hpp"
#include "model/AlarmData.hpp"
#include "model/ClockData.hpp"
#include "model/WeatherData.hpp"
#include "view/manager/ViewManager.h"
#include "view/cli/MainClockView.h"
#include "service/TimeUpdateService.h"


#ifdef _WIN32
#include <windows.h>
#endif

void setup_console() {
#ifdef _WIN32
    // Set windows console to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Get the handle to the standard output device.
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hOut == INVALID_HANDLE_VALUE) {
        exit(GetLastError());
    }

    DWORD dwMode = 0;

    // Get the current output mode.
    if (!GetConsoleMode(hOut, &dwMode)) {
        exit(GetLastError());
    }

    // Enable the virtual terminal processing mode.
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(hOut, dwMode)) {
        exit(GetLastError());
    }
#endif
}

using namespace PiAlarm;

[[ noreturn ]]
int main(int argc, char *argv[]) {
#ifdef DISPLAY_CONSOLE
    setup_console();
#endif

    // Models
    model::AlarmData alarmData;
    model::ClockData clockData;
    model::WeatherData weatherData;

    // Display
#ifdef DISPLAY_SSD1322
    // ...
#elif defined(DISPLAY_CONSOLE)
    DisplayType& display {std::cout};
#endif

    // Views
    view::ViewManager viewManager{display};
    viewManager.addView(std::make_unique<view::cli::MainClockView>(alarmData, clockData, weatherData));

    // Services
    service::TimeUpdateService timeUpdateService(clockData);
    timeUpdateService.start();

    // Main application loop
    while (true) {
        viewManager.loop();

        std::this_thread::sleep_for(std::chrono::milliseconds(333)); // Approx. 30 FPS
    }
}
