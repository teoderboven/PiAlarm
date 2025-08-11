#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <memory>
#include <vector>

#include "display/ViewOutputConfig.h"
#include "input/HasInputEventHandler.h"
#include "view/IView.h"

namespace PiAlarm::view {

    /**
     * @class ViewManager
     * @brief Manages multiple views in the PiAlarm application.
     *
     * This class is responsible for adding, switching, and rendering views.
     * It handles input events and manages the active view's lifecycle.
     *
     * Implements the HasInputEventHandler interface to handle input events.
     * Views can receive input events if they are in control.
     * The main button is used to enter view control mode, allowing the user to interact with the current view.
     * @note The Back button (input::ButtonId::Back) is used to exit view control mode. Not available inside the view control mode.
     */
    class ViewManager : public input::HasInputEventHandler {

        std::vector<std::unique_ptr<IView>> views_; ///< Owned views
        size_t currentViewIndex_ {0}; ///< Index of the active view
        bool viewInControl_ {false}; ///< Flag to indicate if the view is in control of the input
        bool forceRefresh_ {false}; ///< Flag to force refresh the current view at the next loop

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

        /**
         * Handles input events for the view manager.
         * If the view is in control, it delegates the event to the current view.
         * If the back button is pressed, it exits view control mode.
         * @param event The input event to handle.
         */
        void handleInputEvent(const input::InputEvent& event) override;

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

        /**
         * Checks if the active view is valid.
         * A valid active view means there are views present and the current index is within bounds.
         * @return True if there is a valid active view, false otherwise.
         */
        inline bool hasValidActiveView() const;

    };

    // Inline methods implementation

    inline void ViewManager::clearRenderer() const {
#ifdef DISPLAY_SSD1322
        renderer_.clear();
#elif defined(DISPLAY_CONSOLE)
        renderer_.str(""); // Clear the string stream
        renderer_.clear(); // Clear the internal state of the stream

        screen_ << "\033[2J\033[H"; // ANSI escape codes to clear the screen and move cursor to home position
#endif
    }

    inline void ViewManager::flushDisplay() const {
#ifdef DISPLAY_SSD1322
        const gfx::IBuffer& buffer = renderer_.buffer();
        screen_.flush(buffer.data(), buffer.size());
#elif defined(DISPLAY_CONSOLE)
        // screen is typically std::cout
        screen_ << renderer_.str();
        screen_ << std::flush;
#endif
    }

    bool ViewManager::hasValidActiveView() const {
        return !views_.empty() && currentViewIndex_ < views_.size();
    }

} // namespace PiAlarm::view

#endif //VIEWMANAGER_H
