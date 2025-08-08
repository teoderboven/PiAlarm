#include <iomanip>
#include <ranges>

#include "view/console/MainClockView.h"
#include "utils/consoleDisplayUtils.hpp"
#include "utils/WeatherUtils.hpp"
#include "utils/ViewUtils.hpp"

namespace PiAlarm::view::console {

    MainClockView::MainClockView(
        const model::AlarmsData& alarmsData,
        const model::AlarmState& alarmStateData,
        const model::ClockData &clockData,
        const model::CurrentWeatherData &currentWeatherData,
        const model::TemperatureSensorData& temperatureSensorData
        )
        : AbstractObserverView{true},
        alarmsData_{alarmsData},
        alarmStateData_{alarmStateData},
        clockData_{clockData},
        currentWeatherData_{currentWeatherData},
        temperatureSensorData_{temperatureSensorData}
    {
        alarmsData_.addObserver(this);
        alarmStateData_.addObserver(this);
        clockData_.addObserver(this);
        currentWeatherData_.addObserver(this);
        temperatureSensorData_.addObserver(this);
    }

    MainClockView::~MainClockView() {
        alarmsData_.removeObserver(this);
        alarmStateData_.removeObserver(this);
        clockData_.removeObserver(this);
        currentWeatherData_.removeObserver(this);
        temperatureSensorData_.removeObserver(this);
    }

    void MainClockView::refresh() {
        currentTime_ = clockData_.getCurrentTime();

        enabledAlarmCount_ = alarmsData_.enabledAlarmCount();
        const auto nextAlarm {alarmsData_.getNextAlarm(currentTime_)};
        nextAlarmTime_ = nextAlarm ? nextAlarm->getTime() : model::Time(0);
        hasAlarmEnabled_ = nextAlarm ? nextAlarm->isEnabled() : false;

        currentIndoorTemperature_ = temperatureSensorData_.getTemperature();
        currentIndoorHumidity_ = temperatureSensorData_.getHumidity();
        sensorDataValid_ = temperatureSensorData_.isValid();

        currentOutdoorTemperature_ = currentWeatherData_.getTemperature();
        currentOutdoorHumidity_ = currentWeatherData_.getHumidity();
        currentOutdoorPressure_ = currentWeatherData_.getPressure();
        currentWeatherCondition_ = currentWeatherData_.getCondition();
        currentWeatherDataValid_ = currentWeatherData_.isValid();
    }

    void MainClockView::render(RenderType& renderer) {
        std::vector<std::pair<std::string, std::string>> labels = {
            { "Heure actuelle", utils::formattedTime(currentTime_, true, true) },
            { "Etat de l'alarme", getAlarmStatus() },
            { "Nombre d'alarmes actives", std::to_string(enabledAlarmCount_) },
            { "", "" },

            { "Température pièce", formattedTemperature(currentIndoorTemperature_, sensorDataValid_) },
            { "Humidité pièce", formattedHumidity(currentIndoorHumidity_, sensorDataValid_) },
            { "", "" },

            { "Température ext.", formattedTemperature(currentOutdoorTemperature_, currentWeatherDataValid_) },
            { "Humidité ext.", formattedHumidity(currentOutdoorHumidity_, currentWeatherDataValid_) },
            { "Pression atm.", formattedPressure(currentOutdoorPressure_, currentWeatherDataValid_) },
            { "Condition météo", formattedWeatherCondition(currentWeatherCondition_, currentWeatherDataValid_) }
        };

        displayLabels(renderer,labels);
    }

    void MainClockView::displayLabels(RenderType& renderer, const std::vector<std::pair<std::string, std::string>>& labels) {
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

    std::string MainClockView::formattedTemperature(float temperature, bool valid) {
        return utils::formatValue(temperature, valid, 1, "°C", "--.-°C");
    }

    std::string MainClockView::formattedHumidity(float humidity, bool valid) {
        return utils::formatValue(humidity, valid, 0, "%", "--%");
    }

    std::string MainClockView::formattedPressure(float pressure, bool valid) {
        return utils::formatValue(pressure, valid, 1, " hPa", "----.- hPa");
    }

    std::string MainClockView::formattedWeatherCondition(common::WeatherCondition condition, bool valid, const std::string& locale) {
        if (!valid) return "???";

        return utils::getLocalizedWeatherCondition(condition, locale);
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
