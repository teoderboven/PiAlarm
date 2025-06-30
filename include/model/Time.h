#ifndef TIME_H
#define TIME_H

#include <string>
#include <chrono>

namespace PiAlarm::model {

    /**
     * @class Time
     * Class representing a time of day.
     *
     * This class encapsulates the concept of time, allowing for easy manipulation and comparison
     * of time values. It provides methods to convert time to seconds, format it as a string,
     * and compare different Time objects.
     */
    class Time {

        int hour_;
        int minute_;
        int second_;

        /**
         * Converts the time to seconds since midnight.
         * @return Total seconds from midnight.
         */
        int toSeconds() const;

    public:
        /**
         * Constructs a Time object with the specified hour, minute, and second.
         * @param hour Hour of the time (0-23).
         * @param minute Minute of the time (0-59).
         * @param second Second of the time (0-59).
         * @throws std::out_of_range if any parameter is out of its valid range.
         */
        explicit Time(const int& hour = 0, const int& minute = 0, const int& second = 0);

        /**
         * Sets the time to the specified hour, minute, and second.
         * @param hour Hour of the time (0-23).
         * @param minute Minute of the time (0-59).
         * @param second Second of the time (0-59).
         * @throws std::out_of_range if any parameter is out of its valid range.
         */
        void setTime(const int& hour, const int& minute, const int& second);

        /**
         * Gives the hour of the time.
         * @return Hour (0-23).
         */
        inline int hour() const { return hour_; }
        /**
         * Gives the minute of the time.
         * @return Minute (0-59).
         */
        inline int minute() const { return minute_; }
        /**
         * Gives the second of the time.
         * @return Second (0-59).
         */
        inline int second() const { return second_; }

        /**
         * Returns the time as a string in the format "HH:MM:SS".
         * @return Formatted time string.
         */
        std::string toString() const;

        /**
         * Calculates the number of seconds elapsed since another time.
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
        std::chrono::seconds secondsSince(const Time& other) const;

        bool operator==(const Time& other) const;
        bool operator!=(const Time& other) const;
        bool operator<(const Time& other) const;
        bool operator<=(const Time& other) const;
        bool operator>(const Time& other) const;
        bool operator>=(const Time& other) const;
    };

    std::ostream& operator<<(std::ostream& os, const Time& time);

} // namespace PiAlarm::model

#endif // TIME_H
