#include <thread>

#include "utils/consoleUtils.h"
#include "display/DisplayConfig.hpp"
#include "model/AlarmData.hpp"
#include "model/ClockData.hpp"
#include "model/CurrentWeatherData.h"
#include "model/TemperatureSensorData.hpp"
#include "controller/AlarmController.h"
#include "view/manager/ViewManager.h"
#include "view/cli/MainClockView.h"
#include "service/TimeUpdateService.h"
#include "service/WeatherApiService.h"

namespace PiAlarm {
    // Forward declaration of the run function
    [[noreturn]]
    void run(view::ViewManager& viewManager);
}

using namespace PiAlarm;

/**
 * Main function for the PiAlarm application.
 * Initializes models, controllers, providers, views, and services.
 * Starts the main application loop.
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Exit code (never returns)
 */
[[noreturn]]
int main(int argc, char *argv[]) {
#ifdef DISPLAY_CONSOLE
    utils::setup_console();
#endif

    // Models
    model::AlarmData alarm_data;
    model::ClockData clock_data;
    model::CurrentWeatherData currentWeather_data;
    model::TemperatureSensorData temperatureSensor_data;

    // Controllers
    controller::AlarmController alarmController{alarm_data};

    // Providers
    provider::WeatherApiClient api {};

    // Display
#ifdef DISPLAY_SSD1322
    // ...
#elif defined(DISPLAY_CONSOLE)
    DisplayType& display {std::cout};
#endif

    // Views
    view::ViewManager viewManager{display};
    viewManager.addView(
        std::make_unique<view::cli::MainClockView>(
            alarm_data,
            clock_data,
            currentWeather_data,
            temperatureSensor_data
        )
    );

    // Services
    service::TimeUpdateService timeUpdateService {clock_data};
    timeUpdateService.start();
    service::WeatherApiService weatherApiService {currentWeather_data, api};
    weatherApiService.start();

    // Main application loop
    run(viewManager);
}

/**
 * @namespace PiAlarm
 * @brief The main namespace for the PiAlarm application.
 */
namespace PiAlarm {

    /**
     * Runs the main application loop.
     * This function continuously updates the view manager and sleeps for a short duration
     * to control the frame rate.
     *
     * @param viewManager The view manager to update.
     */
    [[noreturn]]
    void run(view::ViewManager& viewManager) {
        while (true) {
            viewManager.loop();

            std::this_thread::sleep_for(std::chrono::milliseconds(333)); // Approx. 30 FPS
        }
    }

} // namespace PiAlarm
