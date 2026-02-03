#include "service/WeatherApiService.h"

namespace PiAlarm::service {

    WeatherApiService::WeatherApiService(
        model::CurrentWeatherData& currentWeatherData,
        const provider::WeatherApiClient& weatherApiClient
    )
        : BaseService{"WeatherApiService"},
          currentWeatherData_{currentWeatherData},
          weatherApiClient_{weatherApiClient}
    {}

    void WeatherApiService::update() {
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

        if (failureCount_ >= WEATHER_API_SERVICE_MAX_FAILURE_COUNT) {
            logger().critical("Failed to fetch weather data {} times in a row. Invalidating model.", WEATHER_API_SERVICE_MAX_FAILURE_COUNT);

            currentWeatherData_.setValid(false);
        }
        else {
            logger().warn("Failed to fetch weather data. Attempt {} of {}.", failureCount_, WEATHER_API_SERVICE_MAX_FAILURE_COUNT);
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

    std::chrono::milliseconds WeatherApiService::getDurationUntilNextAlignment(const int& minuteAlignment){
        using namespace std::chrono;

        // Get current time as system_clock::time_point
        const auto now {floor<seconds>(system_clock::now())};

        // Compute time since beginning of the hour
        const auto minutesSinceHour {duration_cast<minutes>(now.time_since_epoch()) % 60min};
        const auto secondsSinceHour {duration_cast<seconds>(now.time_since_epoch()) % 1min};

        // Compute time until next alignment
        int minutes_to_next {minuteAlignment - (static_cast<int>(minutesSinceHour.count()) % minuteAlignment)};
        if (minutes_to_next == 0) minutes_to_next = minuteAlignment;

        const auto sleepDuration {minutes{minutes_to_next} - secondsSinceHour};

        return duration_cast<milliseconds>(sleepDuration);
    }

} // namespace PiAlarm::service