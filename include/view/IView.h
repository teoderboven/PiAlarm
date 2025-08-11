#ifndef IVIEW_H
#define IVIEW_H

#include "display/ViewOutputConfig.h"
#include "input/HasInputEventHandler.h"

/**
 * @namespace PiAlarm::view
 * @brief Namespace for views in the PiAlarm application.
 *
 * This namespace contains interfaces and classes related to the views of the PiAlarm application.
 */
namespace PiAlarm::view {

    /**
     * @interface IView
     * @brief Interface for views in the PiAlarm application.
     *
     * This interface defines the methods that any view must implement to update and render itself.
     */
    class IView : public input::HasInputEventHandler {
    public:

        /**
         * Virtual destructor for the interface.
         * Ensures proper cleanup of derived classes.
         */
        virtual ~IView() = default;

        /**
         * Updates the view with the latest data.
         * This method should be called to refresh the view's content.
         */
        virtual void refresh() = 0;

        /**
         * Renders the view using the provided renderer.
         * This method should be implemented to draw the view's content on the screen.
         * @param renderer The renderer used to draw the view.
         */
        virtual void render(RenderType& renderer) const = 0;

        /**
         * Checks if the view has undisplayed changes.
         * This method should be used to determine if the view needs to be updated or saved.
         * @return True if the view is dirty (has undisplayed changes), false otherwise.
         */
        [[nodiscard]]
        virtual bool isDirty() const = 0;

        /**
         * Clears the dirty state of the view.
         */
        virtual void clearDirty() = 0;

        /**
         * Handles input events specific to the view.
         * This method should be implemented to respond to user inputs such as button presses.
         * @param event The input event to handle.
         */
        virtual void handleInputEvent(const input::InputEvent& event) override = 0;

    };

} // namespace PiAlarm::view


#endif //IVIEW_H
