#ifndef IVIEW_H
#define IVIEW_H

#include "display/DisplayConfig.hpp"

/**
 * @namespace PiAlarm::view
 * @brief Namespace for the PiAlarm view system.
 *
 * This namespace contains classes and interfaces related to the view management in the PiAlarm application.
 */
namespace PiAlarm::view {

    /**
     * @interface IView
     * @brief Interface for views in the PiAlarm application.
     *
     * This interface defines the methods that any view must implement to update and render itself.
     */
    class IView {
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
         * Renders the view to the specified display.
         * This method should be called to draw the view on the screen.
         * @param display The display where the view will be rendered.
         */
        virtual void render(DisplayType& display) = 0;

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

    };

} // namespace PiAlarm::view


#endif //IVIEW_H
