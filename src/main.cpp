#include <thread>

#include "utils/consoleUtils.h"
#include "display/DisplayConfig.hpp"
#include "model/AlarmData.hpp"
#include "model/ClockData.hpp"
#include "model/TemperatureSensorData.hpp"
#include "controller/AlarmController.h"
#include "view/manager/ViewManager.h"
#include "view/cli/MainClockView.h"
#include "service/TimeUpdateService.h"

using namespace PiAlarm;

[[ noreturn ]]
int main(int argc, char *argv[]) {
#ifdef DISPLAY_CONSOLE
    utils::setup_console();
#endif


    // Models
    model::AlarmData alarm_data;
    model::ClockData clock_data;
    model::TemperatureSensorData temperatureSensor_data;

    // Controllers
    controller::AlarmController alarmController{alarm_data};

    // Display
#ifdef DISPLAY_SSD1322
    // ...
#elif defined(DISPLAY_CONSOLE)
    DisplayType& display {std::cout};
#endif

    // Views
    view::ViewManager viewManager{display};
    viewManager.addView(std::make_unique<view::cli::MainClockView>(alarm_data, clock_data, temperatureSensor_data));

    // Services
    service::TimeUpdateService timeUpdateService(clock_data);
    timeUpdateService.start();

    // Main application loop
    while (true) {
        viewManager.loop();

        std::this_thread::sleep_for(std::chrono::milliseconds(333)); // Approx. 30 FPS
    }
}
