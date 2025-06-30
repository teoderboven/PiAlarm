#ifndef OBSERVABLE_HPP
#define OBSERVABLE_HPP

#include <vector>
#include "Observer.h"

namespace PiAlarm::common {

    /**
     * @class Observable
     * Implements the observable part of the Observer design pattern.
     *
     * This class maintains a list of observers and provides methods to add,
     * remove, and notify them of changes.
     */
    class Observable {
        std::vector<Observer*> observers;

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
        void addObserver(Observer* observer) {
            observers.push_back(observer);
        }

        /**
         * Removes an observer from the list of observers.
         * @param observer The observer to remove.
         */
        void removeObserver(Observer* observer) {
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
