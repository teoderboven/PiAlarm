#include <utility>
#include "view/manager/ViewManager.h"

namespace PiAlarm::view {

    void ViewManager::addView(std::unique_ptr<IView> view) {
        views_.push_back(std::move(view));

        if (currentViewIndex_ == -1) {
            currentViewIndex_ = 0; // Set the first view as active if none is set
        }
    }

    void ViewManager::nextView() {
        if (views_.empty()) {
            return; // No views to switch to
        }

        currentViewIndex_ = (currentViewIndex_ + 1) % static_cast<int>(views_.size());
    }

    void ViewManager::previousView() {
        if (views_.empty()) {
            return;
        }

        auto size = static_cast<int>(views_.size());

        currentViewIndex_ = (currentViewIndex_ + size - 1) % size;
    }

    void ViewManager::loop(DisplayType &display) {
        if (currentViewIndex_ < 0 || currentViewIndex_ >= static_cast<int>(views_.size())) {
            return; // No active view
        }

        IView* activeView = views_[currentViewIndex_].get();
        if (activeView->isDirty()) {
            activeView->refresh();
            activeView->render(display);
            activeView->clearDirty();
        }
    }

} // namespace PiAlarm::view
