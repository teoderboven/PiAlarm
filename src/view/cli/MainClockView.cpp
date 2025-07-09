#include <sstream>
#include "view/cli/MainClockView.h"

namespace PiAlarm::view::cli {

    MainClockView::MainClockView(const model::AlarmData& alarmData, const model::ClockData &clockData, const model::TemperatureSensorData& temperatureSensorData)
        : BaseCliView{true}, alarmData_{alarmData}, clockData_{clockData}, temperatureSensorData_{temperatureSensorData}
    {
        alarmData_.addObserver(this);
        clockData_.addObserver(this);
        temperatureSensorData_.addObserver(this);
    }

    void MainClockView::refresh() {
        currentTime_ = clockData_.getCurrentTime();
        alarmTime_ = alarmData_.getAlarmTime();
        alarmEnabled_ = alarmData_.isAlarmEnabled();
        temperature_ = temperatureSensorData_.getTemperature();
        humidity_ = temperatureSensorData_.getHumidity();
        sensorDataValid = temperatureSensorData_.isValid();
    }

    void MainClockView::render(DisplayType &display) {
        clearDisplay(display);

        display << "Heure actuelle : " << formattedCurrentTime() << std::endl
                << "Réveil à       : " << formattedAlarmTime() << std::endl
                << "Température    : " << formattedTemperature() << std::endl
                << "Humidité       : " << formattedHumidity() << std::endl;
    }

    std::string MainClockView::formattedCurrentTime() const {
        return currentTime_.toString();
    }

    std::string MainClockView::formattedAlarmTime() const {
        return alarmEnabled_ ? alarmTime_.toString() : "--:--";
    }

    std::string MainClockView::formattedTemperature() const {
        if (!sensorDataValid)
            return "--.-°C";

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << temperature_ << "°C";
        return oss.str();
    }

    std::string MainClockView::formattedHumidity() const {
        if (!sensorDataValid)
            return "--%";

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << humidity_ << "%";
        return oss.str();
    }

} // namespace PiAlarm::view::cli
