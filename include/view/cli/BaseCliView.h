#ifndef BASECLIVIEW_H
#define BASECLIVIEW_H

#include <atomic>

#include "common/Observer.h"
#include "view/IView.h"

namespace PiAlarm::view::cli {

    /**
     * @class BaseCliView
     * @brief Base class for CLI views in the PiAlarm application.
     *
     * This class serves as a base for all CLI views, providing common functionality
     * and ensuring that derived classes implement the necessary methods.
     *
     * @note Don't forget to implement the refresh() and render() methods in derived classes.
     */
    class BaseCliView : public IView, public common::Observer {

        std::atomic<bool> dirty_; ///< Flag indicating if the view is dirty (needs to be refreshed)
    public:

        /**
         * Constructor for the BaseCliView.
         * Initializes the dirty state to true by default.
         * @param isDirty Initial dirty state of the view.
         */
        BaseCliView(bool isDirty = true)
            : dirty_(isDirty)
        {}

        /**
         * Virtual destructor for the BaseCliView.
         * Ensures proper cleanup of derived classes.
         */
        virtual ~BaseCliView() override = default;

        /**
         * Clears the display by sending ANSI escape codes.
         * @param display The display to clear.
         */
        inline void clearDisplay(DisplayType& display) const {
            display << "\033[2J\033[H"; // ANSI escape codes to clear the screen and move cursor to home position
        }

        // Inherited from IView
        // Still needs to be implemented by derived classes.
        virtual void refresh() override = 0;
        virtual void render(DisplayType& display) override = 0;

        // Inherited from IView
        // Is implemented here to avoid code duplication in derived classes.
        inline bool isDirty() const override {
            return dirty_.load();
        }

        inline void clearDirty() override {
            dirty_.store(false);
        }

        // Inherited from Observer
        inline void update() override {
            dirty_.store(true); // The model has changed, the view needs to be refreshed
        }

    };

} // namespace PiAlarm::view::cli

#endif //BASECLIVIEW_H
