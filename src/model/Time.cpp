#include <format>
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

    Time::Time(const int& hour, const int& minute, const int& second)
        : hour_{hour}, minute_{minute}, second_{second}
    {
        if (hour < 0 || hour > 23)
            throw std::out_of_range(std::format("Hour must be between 0 and 23, got {}", hour));
        if (minute < 0 || minute > 59)
            throw std::out_of_range(std::format("Minute must be between 0 and 59, got {}", minute));
        if (second < 0 || second > 59)
            throw std::out_of_range(std::format("Second must be between 0 and 59, got {}", second));
    }

    std::chrono::seconds Time::secondsSince(const Time& other) const {
        const int thisSec = toSeconds();
        const int otherSec = other.toSeconds();
        int diff = thisSec - otherSec;
        if (diff < 0)
            diff += 24 * 3600;
        return std::chrono::seconds(diff);
    }

    int Time::toSeconds() const {
        return hour_ * 3600 + minute_ * 60 + second_;
    }

    std::string Time::toString() const {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << hour_ << ":"
            << std::setw(2) << minute_ << ":"
            << std::setw(2) << second_;
        return oss.str();
    }

    bool Time::operator==(const Time& other) const {
        return hour_ == other.hour_ &&
               minute_ == other.minute_ &&
               second_ == other.second_;
    }

    bool Time::operator!=(const Time& other) const {
        return !(*this == other);
    }

    bool Time::operator<(const Time& other) const {
        if (hour_ != other.hour_) return hour_ < other.hour_;
        if (minute_ != other.minute_) return minute_ < other.minute_;
        return second_ < other.second_;
    }

    bool Time::operator<=(const Time& other) const {
        return *this < other || *this == other;
    }

    bool Time::operator>(const Time& other) const {
        return !(*this <= other);
    }

    bool Time::operator>=(const Time& other) const {
        return !(*this < other);
    }

    std::ostream& operator<<(std::ostream& os, const Time& time) {
        return os << time.toString();
    }

} // namespace PiAlarm::model
