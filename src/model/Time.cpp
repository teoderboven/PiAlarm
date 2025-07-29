#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "model/Time.h"

namespace PiAlarm::model {

    Time Time::now() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm* tm = std::localtime(&time);

        return Time(tm->tm_hour, tm->tm_min, tm->tm_sec);
    }

    Time::Time(int hour, int minute, int second)
        : sinceMidnight_{std::chrono::hours(hour) + std::chrono::minutes(minute) + std::chrono::seconds(second)}
    {
        if (hour < 0 || hour > 23) {
            std::ostringstream oss;
            oss << "Hour must be between 0 and 23, got " << hour;
            throw std::out_of_range(oss.str());
        }
        if (minute < 0 || minute > 59) {
            std::ostringstream oss;
            oss << "Minute must be between 0 and 59, got " << minute;
            throw std::out_of_range(oss.str());
        }
        if (second < 0 || second > 59) {
            std::ostringstream oss;
            oss << "Second must be between 0 and 59, got " << second;
            throw std::out_of_range(oss.str());
        }
    }

    Time::Time(std::chrono::seconds sinceMidnight) {
        auto s = sinceMidnight.count();
        // Ensure the seconds are within the range of a day (0 to 86399)
        s = ((s % 86400) + 86400) % 86400;
        sinceMidnight_ = std::chrono::seconds(s);
    }

    bool Time::isBetween(const Time& start, const Time& end) const {
        if (start <= end) {
            return *this >= start && *this < end;
        }
        // if the range wraps around midnight
        return *this >= start || *this < end;
    }

    std::string Time::toString(bool includeSeconds) const {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << hour() << ":"
            << std::setw(2) << minute();

        if (includeSeconds) {
            oss << ":"
                << std::setw(2) << second();
        }

        return oss.str();
    }

} // namespace PiAlarm::model
