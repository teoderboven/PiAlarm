#ifndef OBSERVABLE_HPP
#define OBSERVABLE_HPP

#include <vector>
#include "Observer.h"

namespace PiAlarm::common {

    /**
     * @class Observable
     * @brief The observable part of the Observer design pattern.
     *
     * This class maintains a list of observers and provides methods to add,
     * remove, and notify them of changes.
     */
    class Observable {
        // 'observers' is mutable to allow adding/removing observers from const methods without altering the observable state.
        mutable std::vector<Observer*> observers; ///< The list of observers

    public:

        /**
         * Virtual destructor for the observable.
         * Ensures proper cleanup of derived classes.
         */
        virtual ~Observable() = default;

        /**
         * Adds an observer to the list of observers.
         * @param observer The observer to add.
         */
        void addObserver(Observer* observer) const {
            observers.push_back(observer);
        }

        /**
         * Removes an observer from the list of observers.
         * @param observer The observer to remove.
         */
        void removeObserver(Observer* observer) const {
            std::erase(observers, observer);
        }

        /**
         * Notifies all observers of a change.
         */
        virtual void notifyObservers() {
            for (Observer* observer : observers) {
                observer->update();
            }
        }

    };

} // namespace PiAlarm::common

#endif //OBSERVABLE_HPP
