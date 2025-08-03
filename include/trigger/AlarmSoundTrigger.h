#ifndef ALARMSOUNDTRIGGER_H
#define ALARMSOUNDTRIGGER_H

#include "common/Observer.h"
#include "model/AlarmState.hpp"
#include "media/MusicService.h"

/**
 * @namespace PiAlarm::trigger
 * @brief Namespace for trigger-related classes in the PiAlarm application.
 *
 * This namespace contains classes that handle various triggers, including alarm sound management.
 */
namespace PiAlarm::trigger {

    /**
     * @class AlarmSoundTrigger
     * @brief A trigger that manages the sound playback for alarms based on the current alarm state.
     *
     * This class observes the alarm state and plays the appropriate sound when the alarm is triggered.
     * It uses a MusicService instance to handle audio playback.
     */
    class AlarmSoundTrigger final : public common::Observer {
        const model::AlarmState& alarmState_; ///< The current state of the alarm, used to determine sound behavior.
        media::MusicService& musicService_; ///< Music service to handle audio playback for the alarm sound.

    public:

        /**
         * @brief Constructor for AlarmSoundTrigger.
         * Initializes the trigger with the current alarm state.
         * @param alarmState The model for the current state of the alarm.
         * @param musicService The music service used to play the alarm sound.
         */
        explicit AlarmSoundTrigger(const model::AlarmState& alarmState, media::MusicService& musicService);

        /**
         * @brief Destructor for AlarmSoundTrigger.
         * Cleans up resources used by the trigger.
         */
        ~AlarmSoundTrigger() override;

        /**
         * @brief Updates the trigger based on the current alarm state.
         * This method is called when the observed alarm state changes.
         */
        inline void update() override;

    private:

        /**
         * @brief Handles changes in the alarm state.
         * This method is called when the observed alarm state changes.
         * It determines the appropriate action based on the new state.
         */
        void handleAlarmStateChange() const;

    };

    // Inline method implementation

    inline void AlarmSoundTrigger::update() {
        handleAlarmStateChange();
    }

} // namespace PiAlarm::trigger

#endif //ALARMSOUNDTRIGGER_H
