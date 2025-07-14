#ifndef WEATHERCONDITION_H
#define WEATHERCONDITION_H

namespace PiAlarm::common {

    /**
     * @enum WeatherCondition
     * @brief Enumeration for different weather conditions.
     *
     * This enumeration defines various weather conditions that can be used in the application.
     * It includes conditions like Clear, PartlyCloudy, Cloudy, Fog, Rain (with varying intensities),
     * Snow (with varying intensities), and Thunderstorm.
     */
    enum class WeatherCondition : int8_t {
        Clear,          ///< Clear weather condition
        PartlyCloudy,   ///< Partly cloudy weather condition
        Cloudy,         ///< Cloudy weather condition
        Fog,            ///< Foggy weather condition
        LightRain,      ///< Light rain weather condition
        ModerateRain,   ///< Moderate rain weather condition
        HeavyRain,      ///< Heavy rain weather condition
        Thunderstorm,   ///< Thunderstorm weather condition
        LightSnow,      ///< Light snow weather condition
        ModerateSnow,   ///< Moderate snow weather condition
        HeavySnow,      ///< Heavy snow weather condition
        MixedRainSnow,  ///< Mixed rain and snow weather condition
        Unknown         ///< Unknown weather condition
    };

} // namespace PiAlarm::common

#endif //WEATHERCONDITION_H
