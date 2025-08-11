#include <utility>
#include "view/manager/ViewManager.h"

namespace PiAlarm::view {

    ViewManager::ViewManager(ScreenType& screen, RenderType& renderer)
        : screen_{screen}, renderer_{renderer}
    {}

    void ViewManager::addView(std::unique_ptr<IView> view) {
        views_.push_back(std::move(view));
    }

    void ViewManager::nextView() {
        if (views_.empty()) {
            return; // No views to switch to
        }

        currentViewIndex_ = (currentViewIndex_ + 1) % views_.size();
        forceRefresh_ = true; // Force refresh the view after switching
    }

    void ViewManager::previousView() {
        if (views_.empty()) {
            return;
        }

        currentViewIndex_ = (currentViewIndex_ + views_.size() - 1) % views_.size();
        forceRefresh_ = true;
    }

    void ViewManager::loop() {
        if (!hasValidActiveView()) {
            return;
        }

        IView* activeView = views_[currentViewIndex_].get();
        if (activeView->isDirty() || forceRefresh_) {
            forceRefresh_ = false; // Reset the force refresh flag

            clearRenderer(); // Clear the renderer before rendering the new view

            if (viewInControl_) {
                #ifdef DISPLAY_SSD1322
                    // Draw a border around the view
                    renderer_.drawRectangle(0,0, renderer_.getWidth(), renderer_.getHeight(), 1, 0x80);
                #endif // DISPLAY_SSD1322
            }

            activeView->refresh();
            activeView->render(renderer_);
            activeView->clearDirty();

            flushDisplay(); // Flush the display to show the rendered view
        }
    }

    void ViewManager::handleInputEvent(const input::InputEvent &event) {
        if (!event.pressed) return;

        if (viewInControl_) {
            if (event.button == input::ButtonId::Back) {
                viewInControl_ = false; // Exit view control mode
                forceRefresh_ = true;
            } else {
                // If the view is in control, delegate the input event to the current view
                views_[currentViewIndex_]->handleInputEvent(event);
            }
            return;
        }

        // If the view is not in control, handle navigation
        switch (event.button) {
            case input::ButtonId::Next:
                nextView();
                break;
            case input::ButtonId::Previous:
                previousView();
                break;
            case input::ButtonId::Main:
                viewInControl_ = true; // Enter view control mode
                forceRefresh_ = true;
                break;
            default:
                break; // Ignore other buttons
        }
    }

} // namespace PiAlarm::view
