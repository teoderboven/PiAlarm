#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <memory>
#include <vector>

#include "display/DisplayConfig.hpp"
#include "view/IView.h"

namespace PiAlarm::view {

    /**
     * @class ViewManager
     * @brief Manages multiple views in the application.
     *
     * The ViewManager owns and manages the lifecycle of views.
     * It supports navigation between views and delegates rendering.
     */
    class ViewManager {

        std::vector<std::unique_ptr<IView>> views_; ///< Owned views
        int currentViewIndex_ = -1; ///< Index of the active view (-1 if none)

        DisplayType& display_; ///< Reference to the display for rendering

    public:

        /**
         * Constructor for ViewManager.
         * Initializes the manager with a reference to the display.
         * @param display Reference to the display to use for rendering views.
         */
        ViewManager(DisplayType& display);

        /**
         * Adds a view to the manager.
         * The manager takes ownership of the view.
         * @param view Unique pointer to the view to be added.
         */
        void addView(std::unique_ptr<IView> view);

        /**
         * Switches to the next view in the list.
         * Wraps around to the first view if at the end.
         * Does nothing if no views are present.
         */
        void nextView();

        /**
         * Switches to the previous view in the list.
         * Wraps around to the last view if at the beginning.
         * Does nothing if no views are present.
         */
        void previousView();

        /**
         * Performs the loop operation for the current view.
         * Calls update and render if the view is dirty.
         * If no active view is set, this method does nothing.
         */
        void loop();

    };

} // namespace PiAlarm::view

#endif //VIEWMANAGER_H
