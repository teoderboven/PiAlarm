#include <chrono>

#include "provider/WeatherApiClient.h"
#include "utils/WeatherUtils.hpp"

namespace PiAlarm::provider {

    WeatherApiClient::WeatherApiClient(const std::string& cityName)
      : weatherApiUrl_{WEATHER_API_BASE_URL + cityName}
    {}

    WeatherApiClient::WeatherResult WeatherApiClient::fetchCurrentWeather() const {
        try {
            const auto response = makeRequest();

            auto err = checkForErrors(response);
            if (err.has_value())
                return err.value();

            auto json = parseJson(response.text);
            if (std::holds_alternative<WeatherError>(json))
                return std::get<WeatherError>(json);

            return extractDto(std::get<nlohmann::json>(json));

        } catch (const std::exception& e) {

            return WeatherError{
                .type = WeatherErrorType::Unknown,
                .message = std::string{"Exception: "} + e.what()
            };
        }
    }

    cpr::Response WeatherApiClient::makeRequest() const {
        return cpr::Get(
            weatherApiUrl_,
            cpr::Timeout{std::chrono::seconds(20)},
            cpr::Header{{"Accept", "application/json"}}
        );
    }

    std::optional<WeatherError> WeatherApiClient::checkForErrors(const cpr::Response& response) const {
        if (response.error) {
            return WeatherError{
                .type = WeatherErrorType::NetworkFailure,
                .message = response.error.message
            };
        }

        if (response.status_code != 200) {
            std::ostringstream oss;

            oss << "HTTP code " << response.status_code << " : " << response.text;

            return WeatherError{
                .type = WeatherErrorType::HttpError,
                .message = oss.str()
            };
        }

        return {};
    }

    std::variant<nlohmann::json, WeatherError> WeatherApiClient::parseJson(const std::string& text) const {
        try {
            return nlohmann::json::parse(text);

        } catch (const std::exception& e) {

            return WeatherError{
                .type = WeatherErrorType::JsonParseError,
                .message = std::string{"JSON parse error: "} + e.what()
            };
        }
    }

    WeatherDTO WeatherApiClient::extractDto(const nlohmann::json& json) const {
        // API documentation: https://www.prevision-meteo.ch/uploads/pdf/recuperation-donnees-meteo.pdf

        auto current = json["current_condition"];

        WeatherDTO dto{
            .currentTemperature = current["tmp"].get<float>(),
            .currentHumidity = current["humidity"].get<float>(),
            .currentPressure = current["pressure"].get<float>(),
            .currentCondition = utils::weatherConditionFromKey(current["condition_key"].get<std::string>())
        };

        return dto;
    }

} // namespace PiAlarm::provider
