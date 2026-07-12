#include "service/WeatherApiService.h"

namespace PiAlarm::service {

    WeatherApiService::WeatherApiService(
        model::CurrentWeatherData& currentWeatherData,
        const std::string& cityName
    )
        : BaseService{"WeatherApiService"},
          currentWeatherData_{currentWeatherData},
          weatherApiClient_{cityName}
    {}

    void WeatherApiService::process() {
        const auto result {weatherApiClient_.fetchCurrentWeather()};

        if (std::holds_alternative<provider::WeatherError>(result)) {
            handleClientError(std::get<provider::WeatherError>(result));
        }
        else{
            handleClientResult(std::get<provider::WeatherDTO>(result));
        }
    }

    void WeatherApiService::handleClientError(const provider::WeatherError& error) {
        logger().error("Error fetching weather data: {}", error.message);

        if (error.type == provider::WeatherErrorType::JsonParseError) {
            logger().critical("JsonParseError - Please check the API response format.");
            stop(); // Stop the service if the JSON format is invalid, error will not be resolved by retrying
            return;
        }

        failureCount_++;

        if (failureCount_ >= MAX_FAILURE_COUNT) {
            logger().critical("Failed to fetch weather data {} times in a row. Invalidating model.", MAX_FAILURE_COUNT);

            currentWeatherData_.setValid(false);
        }
        else {
            logger().warn("Failed to fetch weather data. Attempt {} of {}.", failureCount_, MAX_FAILURE_COUNT);
        }
    }

    void WeatherApiService::handleClientResult(const provider::WeatherDTO& result) {
        failureCount_ = 0; // Reset failure count on successful fetch

        currentWeatherData_.setValues(
            result.currentTemperature,
            result.currentHumidity,
            result.currentPressure,
            result.currentCondition
        );
    }

    std::chrono::milliseconds WeatherApiService::getDurationUntilNextAlignment(std::chrono::minutes minuteAlignment) {
        using namespace std::chrono;

        const auto now{floor<seconds>(system_clock::now())};

        const auto minutesSinceHour{duration_cast<minutes>(now.time_since_epoch()) % 1h};
        const auto secondsSinceMinute{duration_cast<seconds>(now.time_since_epoch()) % 1min};

        // Compute time until next alignment
        auto minutesToNext{minuteAlignment - (minutesSinceHour % minuteAlignment)};
        if (minutesToNext == 0min) {
            minutesToNext = minuteAlignment;
        }

        const auto sleepDuration{minutesToNext - secondsSinceMinute};

        return duration_cast<milliseconds>(sleepDuration);
    }

} // namespace PiAlarm::service