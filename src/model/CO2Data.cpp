#include "CO2Data.hpp"

namespace PiAlarm::model {

    CO2Data::CO2Data(uint16_t co2_ppm, bool valid)
        : co2_ppm_{co2_ppm}, valid_{valid} {}

    void CO2Data::setCO2(uint16_t co2_ppm) {
        bool valueChanged = setIfDifferent(co2_ppm_, co2_ppm);
        if (valueChanged) notifyObservers();
    }

    void CO2Data::setValid(bool valid) {
        bool valueChanged = setIfDifferent(valid_, valid);
        if (valueChanged) notifyObservers();
    }

    void CO2Data::setValues(uint16_t co2_ppm, bool valid) {
        bool valueChanged = false;
        {
            std::lock_guard lock{mutex_};
            const bool co2Changed = (co2_ppm_ != co2_ppm);
            const bool validChanged = (valid_ != valid);
            const bool anyChange = co2Changed || validChanged;
            const bool anyValid = valid_ || valid;
            if (anyChange && anyValid) {
                co2_ppm_ = co2_ppm;
                valid_ = valid;
                valueChanged = true;
            }
        }
        if (valueChanged) notifyObservers();
    }

} // namespace PiAlarm::model