#ifndef MODELDATA_HPP
#define MODELDATA_HPP

#include <mutex>

/**
 * @namespace PiAlarm::model
 * @brief Namespace for model classes in the PiAlarm application.
 */
namespace PiAlarm::model {

    /**
     * @class BaseModelData
     * @brief Base class for model data in the PiAlarm application.
     *
     * This class provides a thread-safe mechanism to manage model data.
     */
    class BaseModelData {
    protected:
        mutable std::mutex mutex_; ///< Protects access to data (multithreading)

        /**
         * Default constructor for ModelData.
         */
        BaseModelData() = default;

    public:
        /**
         * Virtual destructor for ModelData.
         */
        virtual ~BaseModelData() = default;

    protected:

        /**
         * Sets a member variable to a new value if it is different from the current value.
         * @param member Reference to the member variable to set.
         * @param newValue The new value to set.
         * @return True if the value was changed, false otherwise.
         */
        template<typename T>
        bool setIfDifferent(T& member, const T& newValue) {
            std::lock_guard lock{mutex_};

            if (member != newValue) {
                member = newValue;
                return true;
            }
            return false;
        }

    };

} // namespace PiAlarm::model

#endif //MODELDATA_HPP
