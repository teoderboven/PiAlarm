#include <sstream>
#include "view/cli/MainClockView.h"

namespace PiAlarm::view::cli {

    MainClockView::MainClockView(const model::ClockData &clockData, const model::WeatherData &weatherData)
        : BaseCliView{true}, clockData_{clockData}, weatherData_{weatherData}
    {
        clockData_.addObserver(this);
        weatherData_.addObserver(this);
    }

    void MainClockView::refresh() {
        currentTime_ = clockData_.getCurrentTime();
        alarmTime_ = clockData_.getAlarmTime();
        alarmEnabled_ = clockData_.isAlarmEnabled();
        temperature_ = weatherData_.getTemperature();
        humidity_ = weatherData_.getHumidity();
        weatherValid_ = weatherData_.isValid();
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
        if (!weatherValid_)
            return "--.-°C";

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << temperature_ << "°C";
        return oss.str();
    }

    std::string MainClockView::formattedHumidity() const {
        if (!weatherValid_)
            return "--.-%";

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << humidity_ << "%";
        return oss.str();
    }

} // namespace PiAlarm::view::cli
