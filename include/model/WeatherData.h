#ifndef WEATHERDATA_H
#define WEATHERDATA_H

namespace PiAlarm::model {

    /**
     * @struct WeatherData
     * Holds weather-related data.
     *
     * This structure stores the current temperature and humidity.
     */
    struct WeatherData {
        /** Current temperature in degrees Celsius */
        float temperature;

        /** Current humidity in percentage */
        float humidity;

        /**
        * Indicates whether the data is valid.
        *
        * This field is `true` if the temperature and humidity values
        * come from a successful sensor reading.
        * It is `false` if the sensor failed to provide reliable data,
        * for example due to a malfunction or read error.
        */
        bool valid = false;
    };


} // namespace PiAlarm::model

#endif //WEATHERDATA_H
