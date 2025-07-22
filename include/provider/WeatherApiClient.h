#ifndef WEATHERAPICLIENT_H
#define WEATHERAPICLIENT_H

#include <string>
#include <optional>
#include <variant>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "common/WeatherCondition.h"
#include "logging/HasLogger.h"


#define WEATHER_API_BASE_URL "https://www.prevision-meteo.ch/services/json/"


/**
 * @namespace PiAlarm::provider
 * @brief Namespace for providers in the PiAlarm application.
 *
 * This namespace contains classes and structures related to external data providers.
 */
namespace PiAlarm::provider {

    /**
     * @struct WeatherDTO
     * @brief Data Transfer Object for weather data.
     *
     * This structure holds the weather data retrieved from an external API.
     */
    struct WeatherDTO {
        float currentTemperature;                    ///< Current temperature
        float currentHumidity;                       ///< Current humidity
        float currentPressure;                       ///< Current atmospheric pressure
        common::WeatherCondition currentCondition;   ///< Current weather condition
    };

    /**
     * @enum WeatherErrorType
     * @brief Enumeration for different types of weatherApi-related errors.
     *
     * This enumeration defines the various error types that can occur when interacting with the weather API.
     */
    enum class WeatherErrorType : int8_t {
        NetworkFailure,  ///< Error due to network issues
        HttpError,       ///< Error due to HTTP response issues (e.g., 404, 500)
        JsonParseError,  ///< Error due to JSON parsing issues
        Unknown          ///< An unknown error occurred
    };

    /**
     * @struct WeatherError
     * @brief Structure representing an error related to weather data retrieval.
     *
     * This structure contains the type of error and a message describing it.
     */
    struct WeatherError {
        WeatherErrorType type;  ///< Type of the error @see WeatherErrorType
        std::string message;    ///< Description of the error
    };

    /**
     * @class WeatherApiClient
     * @brief Client for interacting with a weather API.
     *
     * This class provides methods to fetch weather data from an external API.
     */
    class WeatherApiClient : public logging::HasLogger {

        /**
         * @brief Result type for weather API operations.
         *
         * This type is a variant that can either hold a WeatherDTO on success or a WeatherError on failure.
         */
        using WeatherResult = std::variant<WeatherDTO, WeatherError>;

        const cpr::Url weatherApiUrl_; ///< URL of the weather API endpoint

    public:

        /**
         * @brief Constructs a WeatherApiClient with the specified city name.
         *
         * This constructor initializes the client with the base API URL and the city name for which to fetch weather data.
         *
         * @param cityName The name of the city for which to fetch weather data. Defaults to "Brussel-1".
         *
         * @note You can find the list of available cities at https://www.prevision-meteo.ch/services/json/cities
         */
        explicit WeatherApiClient(const std::string& cityName = "Brussel-1");

        /**
         * @brief Fetches the current weather data.
         *
         * This method makes a request to the weather API and returns the current weather data.
         * If an error occurs, it returns a WeatherError.
         *
         * @return WeatherResult containing either the current weather data or an error.
         */
        [[nodiscard]]
        WeatherResult fetchCurrentWeather() const;

    private:

        /**
         * @brief Makes a request to the weather API.
         *
         * This method performs the actual HTTP request to the weather API and returns the response.
         *
         * @return cpr::Response containing the result of the HTTP request.
         */
        [[nodiscard]]
        inline cpr::Response makeRequest() const;

        /**
         * @brief Checks the response for errors.
         *
         * This method checks the HTTP response for errors and returns a WeatherError if any are found.
         *
         * @param response The HTTP response to check.
         * @return std::optional<WeatherError> containing an error if one occurred, or an empty optional if no errors were found.
         */
        [[nodiscard]]
        static std::optional<WeatherError> checkForErrors(const cpr::Response& response) ;

        /**
         * @brief Parses the JSON response from the weather API.
         *
         * This method attempts to parse the JSON string into a nlohmann::json object.
         * If parsing fails, it returns a WeatherError.
         *
         * @param jsonString The JSON string to parse.
         * @return std::variant<nlohmann::json, WeatherError> containing either the parsed JSON or an error.
         */
        [[nodiscard]]
        static std::variant<nlohmann::json, WeatherError> parseJson(const std::string& jsonString);

        /**
         * @brief Checks the JSON object for API errors.
         *
         * This method checks the JSON object for specific error fields and returns a WeatherError if any are found.
         *
         * @param json The JSON object to check.
         * @return std::optional<WeatherError> containing an error if one occurred, or an empty optional if no errors were found.
         */
        [[nodiscard]]
        static std::optional<WeatherError> checkForErrors(const nlohmann::json& json);

        /**
         * @brief Extracts a WeatherDTO from the JSON object.
         *
         * This method extracts the relevant weather data from the JSON object and returns it as a WeatherDTO.
         *
         * @param json The JSON object containing the weather data.
         * @return WeatherDTO containing the extracted weather data.
         */
        [[nodiscard]]
        static WeatherDTO extractDto(const nlohmann::json& json);

    };

} // namespace PiAlarm::provider

#endif //WEATHERAPICLIENT_H
