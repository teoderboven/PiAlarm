#pragma once

#include "BaseService.h"
#include "model/CurrentWeatherData.h"
#include "provider/WeatherApiClient.h"

namespace PiAlarm::service {

    /**
     * @class WeatherApiService
     * @brief Service for fetching and updating weather data from an external API.
     *
     * This service periodically fetches the weather data using the WeatherApiClient
     * and updates the CurrentWeatherData model with the fetched information.
     */
    class WeatherApiService final : public BaseService {
        model::CurrentWeatherData &currentWeatherData_; ///< Reference to the CurrentWeatherData model to update
        const provider::WeatherApiClient weatherApiClient_; ///< Client for fetching weather data

        static constexpr int MAX_FAILURE_COUNT {2}; ///< Maximum allowed consecutive failures before logging an error
        static constexpr std::chrono::minutes MINUTE_ALIGNMENT {5}; ///< Minute alignment for periodic updates

        int failureCount_ = 0; ///< Counter for consecutive failures in fetching weather data

    public:

        /**
         * @brief Constructs a WeatherApiService that updates the CurrentWeatherData model with weather data.
         * @param currentWeatherData Reference to the CurrentWeatherData model to be updated.
         * @param cityName The name of the city for which to fetch weather data.
         */
        explicit WeatherApiService(
            model::CurrentWeatherData &currentWeatherData,
            const std::string& cityName
        );

    protected:

        /**
         * @brief Fetches the current weather data and updates the CurrentWeatherData model.
         *
         * This method is called periodically to fetch the latest weather data from the API
         * and update the CurrentWeatherData model with the fetched information.
         */
        void process() override;

        /**
         * @brief Returns the update interval for the service.
         *
         * This method calculates the duration until the next alignment based on a specified minute alignment.
         * The service will update at regular intervals aligned to this minute.
         *
         * @return The duration until the next alignment in milliseconds.
         */
        std::chrono::milliseconds updateInterval() const override {
            return getDurationUntilNextAlignment(MINUTE_ALIGNMENT);
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
         * @param minuteAlignment The minute alignment for periodic updates.
         * @return The duration until the next alignment in milliseconds.
         */
        static std::chrono::milliseconds getDurationUntilNextAlignment(std::chrono::minutes minuteAlignment);
    };

} // namespace PiAlarm::service