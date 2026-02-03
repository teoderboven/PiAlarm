#ifndef WEATHERAPISERVICE_H
#define WEATHERAPISERVICE_H

#include "service/BaseService.h"
#include "model/CurrentWeatherData.h"
#include "provider/WeatherApiClient.h"

#ifndef WEATHER_API_SERVICE_MAX_FAILURE_COUNT
    #define WEATHER_API_SERVICE_MAX_FAILURE_COUNT 2
#endif
#ifndef WEATHER_API_SERVICE_MINUTE_ALIGNMENT
    #define WEATHER_API_SERVICE_MINUTE_ALIGNMENT 5
#endif

namespace PiAlarm::service {

    /**
     * @class WeatherApiService
     * @brief Service for fetching and updating weather data from an external API.
     *
     * This service periodically fetches the weather data using the WeatherApiClient
     * and updates the CurrentWeatherData model with the fetched information.
     */
    class WeatherApiService final : public BaseService {
        model::CurrentWeatherData &currentWeatherData_;         ///< Reference to the CurrentWeatherData model to update
        const provider::WeatherApiClient& weatherApiClient_;    ///< Client for fetching weather data

        int failureCount_ = 0;      ///< Counter for consecutive failures in fetching weather data

    public:

        /**
         * @brief Constructs a WeatherApiService that updates the CurrentWeatherData model with weather data.
         * @param currentWeatherData Reference to the CurrentWeatherData model to be updated.
         * @param weatherApiClient Client for fetching weather data from an external API.
         */
        explicit WeatherApiService(
            model::CurrentWeatherData &currentWeatherData,
            const provider::WeatherApiClient& weatherApiClient
        );

    protected:

        /**
         * @brief Fetches the current weather data and updates the CurrentWeatherData model.
         *
         * This method is called periodically to fetch the latest weather data from the API
         * and update the CurrentWeatherData model with the fetched information.
         */
        void update() override;

        /**
         * @brief Returns the update interval for the service.
         *
         * This method calculates the duration until the next alignment based on a specified minute alignment.
         * The service will update at regular intervals aligned to this minute.
         *
         * @return The duration until the next alignment in milliseconds.
         */
        std::chrono::milliseconds updateInterval() const override {
            return getDurationUntilNextAlignment(WEATHER_API_SERVICE_MINUTE_ALIGNMENT);
        }

    private:

        /**
         * @brief Handles errors from the weather API client.
         *
         * This method processes the WeatherError received from the WeatherApiClient
         * and logs the error message.
         *
         * @param error The WeatherError received from the WeatherApiClient.
         */
        void handleClientError(const provider::WeatherError& error);

        /**
         * @brief Handles the successful result from the weather API client.
         *
         * This method processes the WeatherDTO received from the WeatherApiClient
         * and updates the CurrentWeatherData model with the fetched weather information.
         *
         * @param result The WeatherDTO received from the WeatherApiClient.
         */
        void handleClientResult(const provider::WeatherDTO& result);

        /**
         * @brief Calculates the update interval for the service.
         *
         * This method calculates the duration until the next alignment based on a specified minute alignment.
         * The service will update at regular intervals aligned to this minute.
         *
         * @return The duration until the next alignment in milliseconds.
         */
        static std::chrono::milliseconds getDurationUntilNextAlignment(const int& minuteAlignment);

    };

} // namespace PiAlarm::service

#endif //WEATHERAPISERVICE_H
