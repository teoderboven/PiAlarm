#pragma once

#include "controller/AlarmController.h"
#include "display/ViewOutputConfig.h"
#include "logging/HasLogger.h"
#include "media/MusicService.h"
#include "model/AlarmsData.hpp"
#include "model/ClockData.hpp"
#include "model/CO2Data.hpp"
#include "model/CurrentIndoorData.hpp"
#include "model/CurrentWeatherData.h"
#include "model/manager/AlarmManager.h"
#include "service/IService.h"
#include "trigger/AlarmSoundTrigger.h"
#include "view/manager/ViewManager.h"

#include <atomic>
#include <vector>
#include <memory>

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
        : public logging::HasLogger
#ifdef INPUT_GPIO
        , public input::HasInputEventHandler // Inherit from HasInputEventHandler to handle input events
#endif
    {
        static constexpr int BACK_BUTTON_LONG_PRESS_COUNT {4}; ///< Number of back button repeat event to trigger a long press action

        std::atomic<bool> running_ {true}; ///< Atomic flag to control the main application loop

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
         * @brief Initializes the application.
         */
        void init();

        /**
         * @brief Starts the main application loop.
         *
         * This method runs the application, handling input events, updating the display,
         * and managing the state of alarms and weather data.
         */
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
         * @brief Initializes TerminationService and registers a shutdown hook to handle application cleanup.
         */
        void initSignalHandler();

        /**
         * @brief Initializes the input manager and sets up input event handling.
         */
        void initInputs();

        /**
         * @brief Initializes the services for the application.
         */
        void initServices();

        /**
         * @brief Starts all services required by the application.
         */
        void startServices() const;

        /**
         * @brief Stops all services gracefully.
         */
        void stopServices() const;

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
        model::CurrentIndoorData currentIndoor_data;            ///< Current indoor data model
        model::CO2Data co2_data;                                ///< Current CO2 measurement data model

        // model manager
        model::manager::AlarmManager alarmManager;              ///< Alarm manager to handle alarm states and operations
        const model::AlarmState& alarmState;                    ///< Current state of the alarm. Retrieved from the AlarmManager

        // controller
        controller::AlarmController alarmController;            ///< Controller to manage alarm operations and interactions

        // for service
        // TODO: replace with more elegant data retrieval
        const std::string& weatherCityName;                     ///< City name for weather data retrieval, used by the WeatherApiService

        // display
        RenderType renderer;                                    ///< Renderer type for the display output

#ifdef DISPLAY_SSD1322
        ScreenType screen;                                      ///< The SSD1322 display controller
#elif defined(DISPLAY_CONSOLE)
        ScreenType& screen;                                     ///< Reference to the console output stream
#endif

        // view manager
        view::ViewManager viewManager;                          ///< Manages the views in the application

#ifdef INPUT_GPIO

        // input manager
        input::InputManager inputManager;                       ///< Manages user input events

        // input event state
        unsigned backButtonPressedCount = 0;                    ///< Counter for back button presses. Used to handle long presses

#endif // INPUT_GPIO

        // services
        std::vector<std::unique_ptr<service::IService>> services; ///< Vector to hold pointers to all services for easy management

        // media
        media::MusicService musicService;                       ///< Service to handle music playback, especially for alarm sounds

        // trigger
        trigger::AlarmSoundTrigger alarmSoundTrigger;           ///< Trigger to manage alarm sounds based on the alarm state
    };

} // namespace PiAlarm