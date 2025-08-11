#ifndef ALARMSDATA_H
#define ALARMSDATA_H

#include <ostream>
#include <stdexcept>

#include "common/Observable.hpp"
#include "common/Observer.h"
#include "model/Alarm.hpp"
#include "model/Time.h"

namespace PiAlarm::model {

    /**
     * @class AlarmsData
     * @brief Represents a collection of alarms in the PiAlarm application.
     *
     * This class manages multiple Alarm objects, allowing for setting, retrieving,
     * and updating alarms. It also handles saving and loading alarms from a file.
     * It inherits from common::Observable to notify observers of changes.
     */
    class AlarmsData final : public common::Observable, public common::Observer {
        Alarm* alarms_; ///< Pointer to an array of Alarm objects
        const std::size_t alarmCount_; ///< Number of alarms managed by this instance

    public:

        /**
         * @brief Constructs AlarmsData with a specified number of alarms.
         * Attempts to restore configured alarms from a file.
         * If the file does not exist or is invalid, initializes with default alarms.
         * @param alarmCount The number of alarms to initialize.
         * @throws std::invalid_argument if alarmCount is zero.
         */
        explicit AlarmsData(std::size_t alarmCount);

        /**
         * @brief Default destructor for AlarmsData.
         * Cleans up the dynamically allocated alarms array.
         */
        ~AlarmsData() override;

        AlarmsData(const AlarmsData&) = delete; ///< No copy constructor
        AlarmsData& operator=(const AlarmsData&) = delete; ///< No copy assignment operator
        AlarmsData(AlarmsData&&) = delete; ///< No move constructor
        AlarmsData& operator=(AlarmsData&&) = delete; ///< No move assignment operator

        /**
         * @brief Sets the alarm at the specified index with the given time and enabled status.
         * @param index The index of the alarm to set.
         * @param alarmTime The time to set for the alarm.
         * @param enabled True to enable the alarm, false to disable it.
         * @throw std::out_of_range if the index is invalid.
         */
        void setAlarm(std::size_t index, Time alarmTime, bool enabled);

        /**
         * @brief Updates the alarms data save and notifies observers.
         * This method is called to save the current state of alarms to a file
         * and notify all observers of the change.
         */
        void update() override;

        /**
         * @brief Returns the number of alarms.
         * This method provides the total number of alarms managed by this instance.
         * @return The number of alarms.
         */
        inline std::size_t size() const noexcept;

        /**
         * @brief Returns the number of alarms.
         * This method provides the total number of alarms managed by this instance.
         * @return The number of alarms.
         */
        inline std::size_t alarmCount() const noexcept;

        /**
         * @brief Returns the number of enabled alarms.
         * This method counts how many alarms are currently enabled.
         * @return The number of enabled alarms.
         */
        std::size_t enabledAlarmCount() const noexcept;

        /**
         * @brief Retrieves the alarm at the specified index.
         * @param index The index of the alarm to retrieve.
         * @return A reference to the alarm at the specified index.
         * @throws std::out_of_range if the index is invalid.
         */
        inline Alarm& getAlarm(std::size_t index);

        /**
         * @brief Retrieves the alarm at the specified index.
         * @param index The index of the alarm to retrieve.
         * @return A const reference to the alarm at the specified index.
         * @throws std::out_of_range if the index is invalid.
         */
        inline const Alarm& getAlarm(std::size_t index) const;

        /**
         * @brief Retrieves the next enabled alarm based on the current time.
         * This method finds the next alarm that is enabled and has a time greater than the current time.
         *
         * @note This method is used to find the next alarm that should trigger.
         * @see getNextAlarmImpl for the implementation details.
         *
         * @param currentTime The current time to compare against.
         * @return A pointer to the next enabled Alarm, or nullptr if no such alarm exists.
         */
        inline Alarm* getNextAlarm(Time currentTime);

        /**
         * @brief Retrieves the next enabled alarm based on the current time.
         * This method finds the next alarm that is enabled and has a time greater than the current time.
         *
         * @note This method is used to find the next alarm that should trigger.
         * @see getNextAlarmImpl for the implementation details.
         *
         * @param currentTime The current time to compare against.
         * @return A const pointer to the next enabled Alarm, or nullptr if no such alarm exists.
         */
        inline const Alarm* getNextAlarm(Time currentTime) const;

        /**
         * @brief Accesses the alarm at the specified index.
         * This operator provides a convenient way to access alarms using the subscript operator.
         * @param index The index of the alarm to access.
         * @return A reference to the alarm at the specified index.
         */
        inline Alarm& operator[](std::size_t index);

        /**
         * @brief Accesses the alarm at the specified index.
         * This operator provides a convenient way to access alarms using the subscript operator.
         * @param index The index of the alarm to access.
         * @return A const reference to the alarm at the specified index.
         */
        inline const Alarm& operator[](std::size_t index) const;

        /**
         * @brief Returns an iterator to the beginning of the alarms.
         * @return An iterator pointing to the first alarm.
         */
        inline Alarm* begin() noexcept;

        /**
         * @brief Returns a const iterator to the beginning of the alarms.
         * @return A const iterator pointing to the first alarm.
         */
        inline const Alarm* begin() const noexcept;

        /**
         * @brief Returns a const iterator to the beginning of the alarms.
         * @return A const iterator pointing to the first alarm.
         */
        inline const Alarm* cbegin() const noexcept;

        /**
         * @brief Returns an iterator to the end of the alarms.
         * @return An iterator pointing to one past the last alarm.
         */
        inline Alarm* end() noexcept;

        /**
         * @brief Returns a const iterator to the end of the alarms.
         * @return A const iterator pointing to one past the last alarm.
         */
        inline const Alarm* end() const noexcept;

        /**
         * @brief Returns a const iterator to the end of the alarms.
         * @return A const iterator pointing to one past the last alarm.
         */
        inline const Alarm* cend() const noexcept;

    private:

        /**
         * @brief Populates the alarms with default values.
         * This method initializes the alarms_ vector with default alarm times and enabled status.
         * @param startIndex The index from which to start populating alarms.
         */
        void populateAlarms(std::size_t startIndex = 0) const;

        /**
         * @brief Template method to get the next alarm based on the current time.
         * This method is specialized for both Alarm and const Alarm types.
         * It finds the next enabled alarm that has a time greater than the current time.
         * @param currentTime The current time to compare against.
         * @return A pointer to the next enabled Alarm, or nullptr if no such alarm exists.
         */
        template<typename AlarmType>
        AlarmType* getNextAlarmImpl(const Time& currentTime) const;

        /**
         * @brief Loads alarms from a file.
         * This method attempts to read alarms from a file and populate the alarms_ vector.
         * If the file does not exist or is invalid, it returns false.
         * @param alarmsCount The number of alarms expected in the file.
         * @return True if alarms were successfully loaded, false otherwise.
         */
        bool loadFromFile(std::size_t alarmsCount);

        /**
         * @brief Saves the alarms to a file.
         * This method writes the current state of the alarms_ vector to a file.
         */
        void saveToFile();

        /**
         * @brief Checks if the given index is within the valid range of alarms.
         * Throws an exception if the index is out of range.
         * @param index The index to check.
         * @throws std::out_of_range if the index is invalid.
         */
        inline void checkIndex(std::size_t index) const;

    };

    // Inline methods implementations

    inline std::size_t AlarmsData::size() const noexcept {
        return alarmCount_;
    }

    inline std::size_t AlarmsData::alarmCount() const noexcept {
        return alarmCount_;
    }

    inline void AlarmsData::checkIndex(std::size_t index) const {
        if (index >= alarmCount_) {
            std::ostringstream oss;
            oss << "Alarm index " << index << " out of range for alarm count " << alarmCount_ << ".";
            throw std::out_of_range(oss.str());
        }
    }

    inline Alarm& AlarmsData::getAlarm(std::size_t index) {
        checkIndex(index);

        return alarms_[index];
    }

    inline const Alarm& AlarmsData::getAlarm(std::size_t index) const {
        checkIndex(index);

        return alarms_[index];
    }

    inline Alarm* AlarmsData::getNextAlarm(Time currentTime) {
        return getNextAlarmImpl<Alarm>(currentTime);
    }

    template<typename AlarmType>
    AlarmType* AlarmsData::getNextAlarmImpl(const Time& currentTime) const {
        AlarmType* nextAlarm {nullptr};
        std::chrono::seconds minDiff {std::chrono::seconds::max()};

        for (const auto& alarm : *this) {
            if (!alarm.isEnabled()) continue;

            std::chrono::seconds diff {currentTime.secondsUntil(alarm.getTime())};

            if (diff.count() > 0 && diff < minDiff) {
                minDiff = diff;
                /*
                 * We use const_cast here because we are returning a pointer to AlarmType,
                 * which could be either Alarm or const Alarm.
                 * This is safe as we are only reading the alarm data, not modifying it.
                 * Real alarms are non-const, so we can safely cast to non-const.
                 */
                nextAlarm = const_cast<AlarmType*>(&alarm);
            }
        }

        return nextAlarm;
    }

    inline const Alarm* AlarmsData::getNextAlarm(Time currentTime) const {
        return getNextAlarmImpl<const Alarm>(currentTime);
    }

    inline Alarm& AlarmsData::operator[](std::size_t index) {
        return getAlarm(index);
    }

    inline const Alarm& AlarmsData::operator[](std::size_t index) const {
        return getAlarm(index);
    }

    inline Alarm* AlarmsData::begin() noexcept {
        return alarms_;
    }

    inline const Alarm* AlarmsData::begin() const noexcept {
        return alarms_;
    }

    inline const Alarm* AlarmsData::cbegin() const noexcept {
        return alarms_;
    }

    inline Alarm* AlarmsData::end() noexcept {
        return alarms_ + alarmCount_;
    }

    inline const Alarm* AlarmsData::end() const noexcept {
        return alarms_ + alarmCount_;
    }

    inline const Alarm* AlarmsData::cend() const noexcept {
        return alarms_ + alarmCount_;
    }

} // namespace PiAlarm::model

#endif //ALARMSDATA_H
