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
        alarmState{alarmManager.getAlarmState()}, // Current state of the alarm, retrieved from the AlarmManager

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

#ifdef INPUT_GPIO

        // input manager
        mainButtonPin{13},
        backButtonPin{12},
        nextButtonPin{6},
        previousButtonPin{5},

        inputManager{
            {
                {mainButtonPin, input::ButtonId::Main},
                {backButtonPin, input::ButtonId::Back, true},
                {nextButtonPin, input::ButtonId::Next, true},
                {previousButtonPin, input::ButtonId::Previous, true}
            }
        },

#endif // INPUT_GPIO

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
#ifdef INPUT_GPIO
            auto events {inputManager.pollEvents()};
            for (const auto& event : events) {
                handleInputEvent(event);
            }
#endif // INPUT_GPIO

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
                alarmState,
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

#ifdef INPUT_GPIO

    void Application::handleInputEvent(const input::InputEvent& event) {
        auto eventConsumed = handleAlarmControlInput(event);
        if (eventConsumed) return; // If the event was consumed by the alarm control, do not propagate further


    }

    bool Application::handleAlarmControlInput(const input::InputEvent& event) {
        if (!alarmState.hasTriggeredAlarm()) {
            if (backButtonPressedCount) backButtonPressedCount = 0; // Reset back button count if no alarm is active

            return false; // No alarm is currently active, ignore the event
        }

        switch (event.button) {
            case input::ButtonId::Main:
                if (event.pressed && !alarmState.isAlarmSnoozed()) {
                    alarmController.snoozeAlarm();
                    return true;
                }
                break;

            case input::ButtonId::Back:
                if (event.pressed) {
                    ++backButtonPressedCount;

                    if (backButtonPressedCount >= BACK_BUTTON_LONG_PRESS_COUNT) {
                        alarmController.stopAlarm();
                        return true;
                    }
                }else {
                    backButtonPressedCount = 0; // Reset count on release
                }
                break;

            default:
                // Unhandled button event
                break;
        }
        return false; // No action taken
    }

#endif

}
