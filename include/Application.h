#ifndef PIALARM_APPLICATION_H
#define PIALARM_APPLICATION_H

#include "display/ViewOutputConfig.h"
#include "model/AlarmsData.hpp"
#include "model/ClockData.hpp"
#include "model/CurrentWeatherData.h"
#include "model/TemperatureSensorData.hpp"
#include "model/manager/AlarmManager.h"
#include "controller/AlarmController.h"
#include "view/manager/ViewManager.h"
#include "service/TimeUpdateService.h"
#include "service/WeatherApiService.h"
#include "media/MusicService.h"
#include "trigger/AlarmSoundTrigger.h"

#ifdef DISPLAY_SSD1322
    #include "hardware/GPIO.h"
    #include "hardware/SPI.h"
#endif

#ifdef INPUT_GPIO
    #include "input/InputManager.h"
    #include "input/HasInputEventHandler.h"
#endif

/**
 * @namespace PiAlarm
 * @brief The main namespace for the PiAlarm application.
 *
 * This namespace contains the Application class which initializes and runs the PiAlarm application.
 */
namespace PiAlarm {

    /**
     * @class Application
     * @brief The main application class for PiAlarm.
     *
     * This class initializes the models, controllers, providers, views, and services,
     * and starts the main application loop.
     */
    class Application
#ifdef INPUT_GPIO
        : public input::HasInputEventHandler // Inherit from HasInputEventHandler to handle input events
#endif
    {
        static constexpr int BACK_BUTTON_LONG_PRESS_COUNT {4}; ///< Number of back button repeat event to trigger a long press action

    public:

        /**
         * @brief Constructs the PiAlarm Application.
         *
         * This constructor initializes the application with default values for alarm count,
         * snooze duration, ring duration, weather city name, and custom music folder path.
         *
         * @param alarmCount The number of alarms to manage (default is 3)
         * @param snoozeDuration The duration for snooze functionality (default is 5 minutes)
         * @param ringDuration The duration for alarm ringing (default is 60 minutes)
         * @param weatherCityName The city name for weather data retrieval (default is "Brussel-1") (see provider::WeatherApiClient::WeatherApiClient for available cities)
         * @param customMusicFolderPath Path to a custom folder for alarm sounds (default is empty string (The alarmService will use the app default music folder))
         */
        explicit Application(
            size_t alarmCount = 3,
            std::chrono::minutes snoozeDuration = std::chrono::minutes(5),
            std::chrono::minutes ringDuration = std::chrono::minutes(60),
            const std::string &weatherCityName = "Brussel-1",
            const std::filesystem::path &customMusicFolderPath = ""
        );

        /**
         * @brief Starts the main application loop.
         *
         * This method runs the application, handling input events, updating the display,
         * and managing the state of alarms and weather data.
         */
        [[noreturn]]
        void run();

#ifdef INPUT_GPIO

        /**
         * @brief Handles input events from the input manager.
         *
         * This method is called when an input event occurs, allowing the application to respond
         * to user inputs such as button presses.
         *
         * @param event The input event to handle.
         */
        void handleInputEvent(const input::InputEvent& event) override;

        /**
         * @brief Handles specific input events related to alarm control.
         *
         * This method processes input events to control the alarm, such as snoozing or stopping the alarm.
         * It checks the current state of the alarm and performs actions based on the button pressed.
         *
         * @param event The input event to handle.
         * @return True if the event was handled, false otherwise.
         */
        bool handleAlarmControlInput(const input::InputEvent& event);

#endif // INPUT_GPIO


    private:

        /**
         * @brief Starts all services required by the application.
         *
         * This method initializes and starts the time update service and weather API service,
         * which are essential for the application's functionality.
         */
        void startServices();

        /**
         * @brief Stops all services gracefully.
         *
         * This method stops the time update service and weather API service,
         * ensuring that all resources are released properly before the application exits.
         */
        void stopServices();

        /**
         * @brief Initializes the views for the application.
         *
         * This method sets up the views that will be displayed in the application
         * and add them to the view manager.
         */
        void initViews();


        // Definition of types used in the application

        // model
        model::ClockData clock_data;                            ///< Clock data model
        model::AlarmsData alarms_data;                          ///< Alarms data model
        model::CurrentWeatherData currentWeather_data;          ///< Current weather data model
        model::TemperatureSensorData temperatureSensor_data;    ///< Temperature sensor data model

        // model manager
        model::manager::AlarmManager alarmManager;              ///< Alarm manager to handle alarm states and operations
        const model::AlarmState& alarmState;                    ///< Current state of the alarm. Retrieved from the AlarmManager

        // controller
        controller::AlarmController alarmController;            ///< Controller to manage alarm operations and interactions

        // providers
        provider::WeatherApiClient apiClient;                   ///< API client for weather data retrieval

        // display
        RenderType renderer;                                    ///< Renderer type for the display output

#ifdef DISPLAY_SSD1322
        // screen and hardware
        hardware::SPI screenSpi;                                ///< SPI interface for the SSD1322 display
        hardware::GPIO screenDcPin;                             ///< Data/Command pin for the SSD1322 display
        hardware::GPIO screenResetPin;                          ///< Reset pin for the SSD1322 display

        ScreenType screen;                                      ///< The SSD1322 display controller
#elif defined(DISPLAY_CONSOLE)
        ScreenType& screen;                                     ///< Reference to the console output stream
#endif

        // view manager
        view::ViewManager viewManager;                          ///< Manages the views in the application

#ifdef INPUT_GPIO

        // input manager and hardware
        hardware::GPIO mainButtonPin;                           ///< GPIO pin for the main button input
        hardware::GPIO backButtonPin;                           ///< GPIO pin for the back button input
        hardware::GPIO nextButtonPin;                           ///< GPIO pin for the next button input
        hardware::GPIO previousButtonPin;                       ///< GPIO pin for the previous button input

        input::InputManager inputManager;                       ///< Manages user input events

        // input event state
        unsigned backButtonPressedCount = 0;                    ///< Counter for back button presses. Used to handle long presses

#endif // INPUT_GPIO

        // services
        service::TimeUpdateService timeUpdateService;           ///< Service to update the time regularly
        service::WeatherApiService weatherApiService;           ///< Service to fetch weather data from an API using the WeatherApiClient

        // media
        media::MusicService musicService;                       ///< Service to handle music playback, especially for alarm sounds

        // trigger
        trigger::AlarmSoundTrigger alarmSoundTrigger;           ///< Trigger to manage alarm sounds based on the alarm state

    };

} // namespace PiAlarm

#endif //PIALARM_APPLICATION_H
