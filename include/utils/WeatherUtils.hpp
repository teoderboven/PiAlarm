#ifndef WEATHERCONDITIONMAPPER_HPP
#define WEATHERCONDITIONMAPPER_HPP

#include "common/WeatherCondition.h"
#include <string>
#include <unordered_map>

/**
 * @file WeatherUtils.hpp
 * @brief Utility functions for mapping and translating weather conditions.
 *
 * This file provides functions to map weather condition keys from prevision-meteo.ch to the WeatherCondition enum,
 * and to translate these conditions into localized strings based on the provided language code.
 */

namespace PiAlarm::utils {

    ///////////////////
    //    Mapping    //
    ///////////////////

    /**
     * @brief Maps a weather condition key from prevision-meteo.ch to its corresponding WeatherCondition enum value.
     *
     * This function takes a string key representing a weather condition and returns the corresponding
     * WeatherCondition enum value. If the key does not match any known condition, it returns WeatherCondition::Unknown.
     *
     * @param key The string key representing the weather condition.
     * @return WeatherCondition The corresponding WeatherCondition enum value.
     */
    [[nodiscard]]
    inline common::WeatherCondition weatherConditionFromKey(const std::string& key) {
        // API documentation: https://www.prevision-meteo.ch/uploads/pdf/recuperation-donnees-meteo.pdf

        using namespace common;

        static const std::unordered_map<std::string, WeatherCondition> mapping = {
            // Clear
            {"ensoleille", WeatherCondition::Clear},
            {"nuit-claire", WeatherCondition::Clear},
            {"nuit-bien-degagee", WeatherCondition::Clear},

            // PartlyCloudy
            {"ciel-voile", WeatherCondition::PartlyCloudy},
            {"nuit-legerement-voilee", WeatherCondition::PartlyCloudy},
            {"faibles-passages-nuageux", WeatherCondition::PartlyCloudy},
            {"eclaircies", WeatherCondition::PartlyCloudy},
            {"developpement-nuageux", WeatherCondition::PartlyCloudy},
            {"nuit-avec-developpement-nuageux", WeatherCondition::PartlyCloudy},

            // Cloudy
            {"stratus", WeatherCondition::Cloudy},
            {"stratus-se-dissipant", WeatherCondition::Cloudy},
            {"nuit-nuageuse", WeatherCondition::Cloudy},
            {"fortement-nuageux", WeatherCondition::Cloudy},
            {"couvert-avec-averses", WeatherCondition::Cloudy},
            {"faiblement-nuageux", WeatherCondition::Cloudy},
            {"nuit-claire-et-stratus", WeatherCondition::Cloudy},

            // Fog
            {"brouillard", WeatherCondition::Fog},

            // Rain
            {"averses-de-pluie-faible", WeatherCondition::LightRain},
            {"nuit-avec-averses", WeatherCondition::LightRain},
            {"pluie-faible", WeatherCondition::LightRain},

            {"averses-de-pluie-moderee", WeatherCondition::ModerateRain},
            {"pluie-moderee", WeatherCondition::ModerateRain},

            {"averses-de-pluie-forte", WeatherCondition::HeavyRain},
            {"pluie-forte", WeatherCondition::HeavyRain},

            // Thunderstorm
            {"faiblement-orageux", WeatherCondition::Thunderstorm},
            {"nuit-faiblement-orageuse", WeatherCondition::Thunderstorm},
            {"orage-modere", WeatherCondition::Thunderstorm},
            {"fortement-orageux", WeatherCondition::Thunderstorm},

            // Snow
            {"averses-de-neige-faible", WeatherCondition::LightSnow},
            {"nuit-avec-averses-de-neige-faible", WeatherCondition::LightSnow},
            {"neige-faible", WeatherCondition::LightSnow},

            {"neige-moderee", WeatherCondition::ModerateSnow},
            {"neige-forte", WeatherCondition::HeavySnow},

            // Mixed
            {"pluie-et-neige-melee-faible", WeatherCondition::MixedRainSnow},
            {"pluie-et-neige-melee-moderee", WeatherCondition::MixedRainSnow},
            {"pluie-et-neige-melee-forte", WeatherCondition::MixedRainSnow},
        };

        std::string keyLower;
        keyLower.reserve(key.size());
        for (const char c : key) keyLower += std::tolower(c);

        auto it = mapping.find(keyLower);
        return (it != mapping.end()) ? it->second : WeatherCondition::Unknown;
    }


    ///////////////////
    //  Translation  //
    ///////////////////

    using TranslationMap = std::unordered_map<common::WeatherCondition, std::string>;
    using enum common::WeatherCondition;

    inline const TranslationMap frenchWeatherDescriptions = {
        {Clear, "Dégagé"},
        {PartlyCloudy, "Partiellement nuageux"},
        {Cloudy, "Nuageux"},
        {Fog, "Brouillard"},
        {LightRain, "Pluie légère"},
        {ModerateRain, "Pluie modérée"},
        {HeavyRain, "Forte pluie"},
        {Thunderstorm, "Orage"},
        {LightSnow, "Neige légère"},
        {ModerateSnow, "Neige modérée"},
        {HeavySnow, "Forte neige"},
        {MixedRainSnow, "Pluie et neige mêlées"},
        {Unknown, "Inconnu"}
    };

    inline const TranslationMap englishWeatherDescriptions = {
        {Clear, "Clear"},
        {PartlyCloudy, "Partly Cloudy"},
        {Cloudy, "Cloudy"},
        {Fog, "Fog"},
        {LightRain, "Light Rain"},
        {ModerateRain, "Moderate Rain"},
        {HeavyRain, "Heavy Rain"},
        {Thunderstorm, "Thunderstorm"},
        {LightSnow, "Light Snow"},
        {ModerateSnow, "Moderate Snow"},
        {HeavySnow, "Heavy Snow"},
        {MixedRainSnow, "Mixed Rain and Snow"},
        {Unknown, "Unknown"}
    };

    /**
     * @brief Localizes a WeatherCondition to its string representation based on the provided translation map.
     *
     * This function looks up the WeatherCondition in the provided translation map and returns the corresponding string.
     * If the condition is not found, it returns "???".
     *
     * @param condition The WeatherCondition to localize.
     * @param translations The translation map to use for localization.
     * @return std::string The localized weather condition string.
     */
    inline std::string localizeWeatherCondition(const common::WeatherCondition& condition, const TranslationMap& translations) {
        const auto it = translations.find(condition);
        return (it != translations.end()) ? it->second : "???";
    }

    /**
     * @brief Gets the localized weather condition string based on the provided language code.
     *
     * This function checks the language code and returns the localized weather condition string
     * using the appropriate translation map. If the language code is "fr", it uses French translations;
     * otherwise, it defaults to English translations.
     *
     * @param condition The WeatherCondition to localize.
     * @param langCode The language code ("fr" for French, "en" for English, ...).
     * @return std::string The localized weather condition string.
     */
    inline std::string getLocalizedWeatherCondition(const common::WeatherCondition& condition, const std::string& langCode) {
        if (langCode == "fr") return localizeWeatherCondition(condition, frenchWeatherDescriptions);
        return localizeWeatherCondition(condition, englishWeatherDescriptions); // English fallback
    }

} // namespace PiAlarm::utils

#endif //WEATHERCONDITIONMAPPER_HPP
