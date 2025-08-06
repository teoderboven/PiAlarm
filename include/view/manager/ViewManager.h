#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <memory>
#include <vector>

#include "display/ViewOutputConfig.h"
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

        ScreenType& screen_; ///< Reference to the screen for rendering views
        RenderType& renderer_; ///< Reference to the renderer for drawing views

    public:

        /**
         * Constructor for ViewManager.
         * Initializes the manager with a screen and renderer.
         * @param screen Reference to the screen where views will be rendered.
         * @param renderer Reference to the renderer used for drawing views.
         */
        ViewManager(ScreenType& screen, RenderType& renderer);

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

    private:

        /**
         * Clears the renderer's buffer.
         * This method is called before rendering a new view to ensure a clean slate.
         */
        inline void clearRenderer() const;

        /**
         * Flushes the display to ensure all changes are rendered.
         * This method is called after rendering the current view.
         */
        inline void flushDisplay() const;

    };

    // Inline methods implementation

    inline void ViewManager::clearRenderer() const {
#ifdef SSD1322_DISPLAY
        renderer_.clear();
#elif defined(DISPLAY_CONSOLE)
        renderer_.str(""); // Clear the string stream
        renderer_.clear(); // Clear the internal state of the stream

        screen_ << "\033[2J\033[H"; // ANSI escape codes to clear the screen and move cursor to home position
#endif
    }

    inline void ViewManager::flushDisplay() const {
#ifdef SSD1322_DISPLAY
        screen_.flush(renderer_.getBuffer());
#elif defined(DISPLAY_CONSOLE)
        // screen is typically std::cout
        screen_ << renderer_.str();
        screen_ << std::flush;
#endif
    }

} // namespace PiAlarm::view

#endif //VIEWMANAGER_H
