#include <iomanip>
#include <ranges>

#include "view/console/MainClockView.h"
#include "utils/consoleDisplayUtils.hpp"
#include "utils/WeatherUtils.hpp"
#include "utils/ViewFormatUtils.hpp"

namespace PiAlarm::view::console {

    MainClockView::MainClockView(
        const model::AlarmsData& alarmsData,
        const model::AlarmState& alarmStateData,
        const model::ClockData &clockData,
        const model::CurrentWeatherData &currentWeatherData,
        const model::CurrentIndoorData& temperatureSensorData
        )
        : AbstractMainClockView(
            alarmsData,
            alarmStateData,
            clockData,
            currentWeatherData,
            temperatureSensorData
        )
        {}

    void MainClockView::render(RenderType& renderer) const {
        std::vector<std::pair<std::string, std::string>> labels = {
            { "Heure actuelle", utils::formatTime(currentTime_, true, true) },
            { "Etat de l'alarme", getAlarmStatus() },
            { "Nombre d'alarmes actives", std::to_string(enabledAlarmCount_) },
            { "", "" },

            { "Température pièce", utils::formatTemperature(currentIndoorTemperature_, indoorDataValid_) },
            { "Humidité pièce", utils::formatHumidity(currentIndoorHumidity_, indoorDataValid_) },
            { "", "" },

            { "Température ext.", utils::formatTemperature(currentOutdoorTemperature_, currentWeatherDataValid_) },
            { "Humidité ext.", utils::formatHumidity(currentOutdoorHumidity_, currentWeatherDataValid_) },
            { "Pression atm.", utils::formatPressure(currentOutdoorPressure_, currentWeatherDataValid_) },
            { "Condition météo", utils::formattedWeatherCondition(currentWeatherCondition_, currentWeatherDataValid_) }
        };

        displayLabels(renderer,labels);
    }

    void MainClockView::displayLabels(RenderType& renderer, const std::vector<std::pair<std::string, std::string>>& labels) const {
        size_t max_len = 0;
        for (const auto &label: labels | std::views::keys) {

            if (label.length() > max_len) max_len = label.length();
        }

        for (const auto& [label, value] : labels) {
            if (label.empty()) {
                renderer << '\n'; // Empty line for spacing
                continue;
            }

            /*
             * special chars like "é", "à" can take more than one byte in UTF-8,
             * so we need to count them to align properly
             */
            auto alignmentWidth = static_cast<int>(max_len + utils::countMultibyteChars(label));

            renderer << std::setfill(' ') << std::setw(alignmentWidth) << std::right << label
                    << " : "
                    << value
                    << '\n';
        }
    }

    std::string MainClockView::getAlarmStatus() const {
        if (!hasAlarmEnabled_)
            return "Aucune alarme activée";

        if (!alarmStateData_.hasTriggeredAlarm())
            return "Prochaine alarme à " + nextAlarmTime_.toString();

        if (alarmStateData_.isAlarmRinging())
            return "Alarme en cours";

        if (alarmStateData_.isAlarmSnoozed())
            return "Alarme en pause jusqu'à " + alarmStateData_.getSnoozeUntil()->toString();

        return "???";
    }

} // namespace PiAlarm::view::console
