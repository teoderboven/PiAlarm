#ifndef ABSTRACTOBSERVERVIEW_H
#define ABSTRACTOBSERVERVIEW_H

#include <atomic>

#include "common/Observer.h"
#include "view/IView.h"

namespace PiAlarm::view {

    /**
     * @class AbstractObserverView
     * @brief Base class for views that observe changes in the model.
     *
     * This class implements the IView interface and extends the Observer class.
     * It provides a mechanism to mark the view as dirty when the model changes,
     * indicating that the view needs to be refreshed.
     *
     * @note Still needs to implement the refresh() and render() methods in derived classes.
     */
    class AbstractObserverView : public IView, public common::Observer {
        std::atomic<bool> dirty_; ///< Flag indicating if the view is dirty (needs to be refreshed)

    public:

        /**
         * Constructor for the BaseCliView.
         * Initializes the dirty state to true by default.
         * @param isDirty Initial dirty state of the view.
         */
        explicit AbstractObserverView(bool isDirty = true)
            : IView{}, dirty_(isDirty)
        {}

        /**
         * Virtual destructor for the BaseCliView.
         * Ensures proper cleanup of derived classes.
         */
        virtual ~AbstractObserverView() override = default;

        // Inherited from IView
        // Still needs to be implemented by derived classes.
        virtual void refresh() override = 0;
        virtual void render(RenderType& renderer) const override = 0;

        // Inherited from IView
        // Is implemented here to avoid code duplication in derived classes.

        [[nodiscard]]
        inline bool isDirty() const override;

        inline void clearDirty() override;

        // Inherited from Observer
        inline void update() override;

        // Inherited from HasInputEventHandler
        void handleInputEvent(const input::InputEvent& event) override {
            // Default implementation does nothing, can be overridden in derived classes
        }

    };

    // inline methods implementation

    inline bool AbstractObserverView::isDirty() const {
        return dirty_.load();
    }

    inline void AbstractObserverView::clearDirty() {
        dirty_.store(false);
    }

    inline void AbstractObserverView::update() {
        dirty_.store(true); // The model has changed, the view needs to be refreshed
    }

} // namespace PiAlarm::view

#endif //ABSTRACTOBSERVERVIEW_H
