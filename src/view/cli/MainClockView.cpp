#include <sstream>
#include <iomanip>
#include "view/cli/MainClockView.h"
#include "utils/WeatherUtils.hpp"

namespace PiAlarm::view::cli {

    MainClockView::MainClockView(
        const model::AlarmsData& alarmsData,
        const model::ClockData &clockData,
        const model::CurrentWeatherData &currentWeatherData,
        const model::TemperatureSensorData& temperatureSensorData
        )
        : BaseCliView{true},
        alarmsData_{alarmsData},
        clockData_{clockData},
        currentWeatherData_{currentWeatherData},
        temperatureSensorData_{temperatureSensorData}
    {
        alarmsData_.addObserver(this);
        clockData_.addObserver(this);
        currentWeatherData_.addObserver(this);
        temperatureSensorData_.addObserver(this);
    }

    void MainClockView::refresh() {
        currentTime_ = clockData_.getCurrentTime();

        const auto nextAlarm {alarmsData_.getNextAlarm(currentTime_)};
        nextAlarmTime_ = nextAlarm ? nextAlarm->getTime() : model::Time();
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

    void MainClockView::render(DisplayType &display) {
        clearDisplay(display);

        display << "Heure actuelle    : " << formattedTime(currentTime_) << "\n"
                << "Prochain réveil à : " << formattedTime(nextAlarmTime_, hasAlarmEnabled_) << "\n"
                << "Température pièce : " << formattedTemperature(currentIndoorTemperature_, sensorDataValid_) << "\n"
                << "Humidité pièce    : " << formattedHumidity(currentIndoorHumidity_, sensorDataValid_) << "\n"
                << "\n"
                << "Température ext.  : " << formattedTemperature(currentOutdoorTemperature_, currentWeatherDataValid_) << "\n"
                << "Humidité ext.     : " << formattedHumidity(currentOutdoorHumidity_, currentWeatherDataValid_) << "\n"
                << "Pression atm.     : " << formattedPressure(currentOutdoorPressure_, currentWeatherDataValid_) << "\n"
                << "Condition météo   : " << formattedWeatherCondition(currentWeatherCondition_, currentWeatherDataValid_) << std::endl;
    }


    std::string MainClockView::formattedTime(const model::Time& time, bool displayTime) {
        return displayTime ? time.toString() : "--:--";
    }

    std::string MainClockView::formatValue(float value, bool valid, int precision, const std::string& unit, const std::string& placeholder) {
        if (!valid) return placeholder;

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << value << unit;
        return oss.str();
    }

    std::string MainClockView::formattedTemperature(float temperature, bool valid) {
        return formatValue(temperature, valid, 1, "°C", "--.-°C");
    }

    std::string MainClockView::formattedHumidity(float humidity, bool valid) {
        return formatValue(humidity, valid, 0, "%", "--%");
    }

    std::string MainClockView::formattedPressure(float pressure, bool valid) {
        return formatValue(pressure, valid, 1, " hPa", "----.- hPa");
    }

    std::string MainClockView::formattedWeatherCondition(common::WeatherCondition condition, bool valid, const std::string& locale) {
        if (!valid) return "???";

        return utils::getLocalizedWeatherCondition(condition, locale);
    }

} // namespace PiAlarm::view::cli
