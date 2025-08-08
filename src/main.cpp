#include <thread>

#include "utils/consoleUtils.hpp"
#include "display/ViewOutputConfig.h"
#include "model/AlarmsData.hpp"
#include "model/ClockData.hpp"
#include "model/CurrentWeatherData.h"
#include "model/TemperatureSensorData.hpp"
#include "model/manager/AlarmManager.h"
#include "controller/AlarmController.h"
#include "view/manager/ViewManager.h"
#include "view/console/MainClockView.h"
#include "service/TimeUpdateService.h"
#include "service/WeatherApiService.h"
#include "media/MusicService.h"
#include "trigger/AlarmSoundTrigger.h"

#ifdef DISPLAY_SSD1322
    #include "hardware/GPIO.h"
    #include "hardware/SPI.h"
    #include "hardware/SSD1322.h"
    #include "gfx/SDD1322Buffer.h"
    #include "gfx/Canvas.h"
    #include "view/ssd1322/MainClockView.h"
#endif

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
    model::ClockData clock_data;
    model::AlarmsData alarms_data{3};
    model::CurrentWeatherData currentWeather_data;
    model::TemperatureSensorData temperatureSensor_data;

    // Model Manager
    model::manager::AlarmManager alarmManager{
        clock_data,
        alarms_data,
        std::chrono::minutes{5}, // Snooze duration
        std::chrono::minutes{60} // Ring duration
    };

    // Controllers
    controller::AlarmController alarmController{alarms_data, alarmManager};

    // Providers
    provider::WeatherApiClient api {};

    // Display
#ifdef DISPLAY_SSD1322
    hardware::SPI oledSpi{"/dev/spidev0.0", 10'000'000}; // SPI device for OLED display
    hardware::GPIO oledDcPin{25}; // GPIO pin for Data/Command selection
    hardware::GPIO oledResetPin{24}; // GPIO pin for reset

    RenderType renderer {std::make_unique<gfx::SDD1322Buffer>()}; // gfx::Canvas
    ScreenType screen {oledSpi, oledDcPin, oledResetPin}; // SSD1322 OLED display

    screen.initialize();
#elif defined(DISPLAY_CONSOLE)
    RenderType renderer{}; // std::ostringstream
    ScreenType& screen{std::cout}; // Console output stream
#endif

    // Views
    view::ViewManager viewManager{screen, renderer};

    // Add views to the view manager
#ifdef DISPLAY_SSD1322
    viewManager.addView(
        std::make_unique<view::ssd1322::MainClockView>(
            alarms_data,
            alarmManager.getAlarmState(),
            clock_data,
            currentWeather_data,
            temperatureSensor_data
        )
    );
#elif defined(DISPLAY_CONSOLE)
    viewManager.addView(
        std::make_unique<view::console::MainClockView>(
            alarms_data,
            alarmManager.getAlarmState(),
            clock_data,
            currentWeather_data,
            temperatureSensor_data
        )
    );
#endif

    // Services
    service::TimeUpdateService timeUpdateService {clock_data};
    timeUpdateService.start();
    service::WeatherApiService weatherApiService {currentWeather_data, api};
    weatherApiService.start();

    // Music Service to play alarm sounds
    media::MusicService musicService{"", "assets/default_alarm"};

    // Triggers
    trigger::AlarmSoundTrigger alarmSoundTrigger{alarmManager.getAlarmState(), musicService};

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
