#include "trigger/AlarmSoundTrigger.h"

namespace PiAlarm::trigger {

    AlarmSoundTrigger::AlarmSoundTrigger(const model::AlarmState &alarmState, media::MusicService &musicService)
        : alarmState_{alarmState}, musicService_{musicService}
    {
        alarmState_.addObserver(this);
    }

    AlarmSoundTrigger::~AlarmSoundTrigger() {
        alarmState_.removeObserver(this);
    }

    void AlarmSoundTrigger::handleAlarmStateChange() const {
        const bool shouldPlayMusic = alarmState_.hasTriggeredAlarm() && alarmState_.isAlarmRinging();
        const bool musicRunning = musicService_.isRunning();

        if (shouldPlayMusic && !musicRunning) {
            musicService_.start();
        } else if (!shouldPlayMusic && musicRunning) {
            musicService_.stop();
        }
    }

} // namespace PiAlarm::trigger
