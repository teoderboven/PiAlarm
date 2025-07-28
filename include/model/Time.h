#ifndef TIME_H
#define TIME_H

#include <string>
#include <chrono>

namespace PiAlarm::model {

    /**
     * @class Time
     * @brief Represents a time of day in hours, minutes, and seconds.
     *
     * This class encapsulates a time of day, allowing for easy manipulation and comparison
     * of times. It provides methods to get the current time, construct a time from hour,
     * minute, and second, and perform various time-related calculations.
     */
    class Time {
        std::chrono::seconds sinceMidnight_; ///< Total seconds since midnight

    public:

        /**
         * @brief Returns the current system time as a Time object.
         * This method retrieves the current local time and constructs a Time object from it.
         * @return A Time object representing the current local time.
         */
        static Time now();

        /**
         * @brief Constructs a Time object with the specified hour, minute, and second.
         * @param hour Hour of the time (0-23).
         * @param minute Minute of the time (0-59).
         * @param second Second of the time (0-59).
         * @throws std::out_of_range if any parameter is out of its valid range.
         */
        explicit Time(int hour = 0, int minute = 0, int second = 0);

        /**
         * @brief Constructs a Time object from a duration since midnight.
         * @param seconds Duration in seconds since midnight (0-86399).
         * @throws std::out_of_range if seconds is not in the range [0, 86399].
         */
        explicit Time(std::chrono::seconds seconds);

        /**
         * @brief Gives the hour of the time.
         * @return Hour (0-23).
         */
        [[nodiscard]]
        inline int hour() const;

        /**
         * @brief Gives the minute of the time.
         * @return Minute (0-59).
         */
        [[nodiscard]]
        inline int minute() const;

        /**
         * @brief Gives the second of the time.
         * @return Second (0-59).
         */
        [[nodiscard]]
        inline int second() const;

        /**
         * @brief Converts the time to a formatted string.
         * Converts the time to a string in the format "HH:MM" or "HH:MM:SS"
         * @param includeSeconds If true, includes seconds in the output string. Default is true.
         * @return The formatted time string.
         */
        [[nodiscard]]
        std::string toString(bool includeSeconds = true) const;

        /**
         * @brief Calculates the number of seconds elapsed since another time.
         *
         * This method returns the elapsed time in seconds between a reference time (`other`)
         * and the current time (`*this`). It assumes a 24-hour cyclical day. If `*this`
         * represents an earlier time than `other`, the calculation wraps around midnight.
         *
         * @param other The reference time to compare with.
         * @return std::chrono::seconds Number of seconds elapsed since `other`.
         *
         * @note The result is always non-negative and ranges from 0 to 86399 seconds.
         */
        [[nodiscard]]
        inline std::chrono::seconds secondsSince(const Time& other) const;

        /**
         * @brief Calculates the number of seconds until another time.
         *
         * This method returns the remaining time in seconds until a reference time (`other`)
         * from the current time (`*this`). It assumes a 24-hour cyclical day. If `*this`
         * represents a later time than `other`, the calculation wraps around midnight.
         *
         * @param other The reference time to compare with.
         * @return std::chrono::seconds Number of seconds until `other`.
         *
         * @note The result is always non-negative and ranges from 0 to 86399 seconds.
         */
        [[nodiscard]]
        inline std::chrono::seconds secondsUntil(const Time& other) const;

        inline bool operator==(const Time& other) const; ///< Checks if two Time objects are equal.
        inline bool operator!=(const Time& other) const; ///< Checks if two Time objects are not equal.

    };

    std::ostream& operator<<(std::ostream& os, const Time& time);

    // Inline methods implementation

    inline int Time::hour() const {
        return static_cast<int>( std::chrono::duration_cast< std::chrono::hours >(this->sinceMidnight_).count() );
    }

    inline int Time::minute() const {
        return static_cast<int>( std::chrono::duration_cast< std::chrono::minutes >(this->sinceMidnight_).count() ) % 60;
    }

    inline int Time::second() const {
        return static_cast<int>( sinceMidnight_.count() ) % 60;
    }

    inline std::chrono::seconds Time::secondsSince(const Time& other) const {
        auto _24hour_seconds = std::chrono::seconds(24 * 3600);
        return (sinceMidnight_ - other.sinceMidnight_ + _24hour_seconds) % _24hour_seconds;
    }

    inline std::chrono::seconds Time::secondsUntil(const Time& other) const {
        return other.secondsSince(*this); // Since + Until = 24 hours
    }

    inline bool Time::operator==(const Time& other) const {
        return sinceMidnight_ == other.sinceMidnight_;
    }

    inline bool Time::operator!=(const Time& other) const {
        return sinceMidnight_ != other.sinceMidnight_;
    }

    inline std::ostream& operator<<(std::ostream& os, const Time& time) {
        return os << time.toString();
    }

} // namespace PiAlarm::model

#endif // TIME_H
