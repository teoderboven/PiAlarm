#include <thread>
#include "Application.h"

#ifdef DISPLAY_SSD1322
    #include "gfx/SDD1322Buffer.h"
    #include "view/ssd1322/MainClockView.h"
#elif defined(DISPLAY_CONSOLE)
    #include "view/console/MainClockView.h"
#endif

namespace PiAlarm {

    Application::Application(
        size_t alarmCount,
        std::chrono::minutes snoozeDuration,
        std::chrono::minutes ringDuration,
        const std::string &weatherCityName,
        const std::filesystem::path &customMusicFolderPath
    )
        // model
      : clock_data{},
        alarms_data{alarmCount},
        currentWeather_data{},
        temperatureSensor_data{},

        // manager
        alarmManager{clock_data, alarms_data, snoozeDuration, ringDuration},

        // controller
        alarmController{alarms_data, alarmManager},

        // provider
        apiClient{weatherCityName},

#ifdef DISPLAY_SSD1322

        // display
        renderer{std::make_unique<gfx::SDD1322Buffer>()},

        // screen hardware
        screenSpi{"/dev/spidev0.0", 10'000'000},
        screenDcPin{25},
        screenResetPin{24},

        screen{screenSpi, screenDcPin, screenResetPin}, // SSD1322 OLED display

#elif defined(DISPLAY_CONSOLE)

        // display
        renderer{},
        screen{std::cout},

#endif // DISPLAY_SSD1322 DISPLAY_CONSOLE

        // view
        viewManager{screen, renderer},

        // service
        timeUpdateService{clock_data},
        weatherApiService{currentWeather_data, apiClient},

        // media
        musicService{customMusicFolderPath, "assets/default_alarm"},

        // trigger
        alarmSoundTrigger{alarmManager.getAlarmState(), musicService}
    {
        initViews();
    }

    void Application::run() {
        startServices();

        while (true) {
            viewManager.loop();
            std::this_thread::sleep_for(std::chrono::milliseconds(333));
        }

        stopServices(); // will never be reached, but good practice to have it here
    }

    void Application::startServices() {
        timeUpdateService.start();
        weatherApiService.start();
    }

    void Application::stopServices() {
        timeUpdateService.stop();
        weatherApiService.stop();
    }

    void Application::initViews() {
#ifdef DISPLAY_SSD1322

        screen.initialize();

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

#endif // DISPLAY_SSD1322 DISPLAY_CONSOLE
    }

}
