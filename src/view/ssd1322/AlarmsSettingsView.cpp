#include "view/ssd1322/AlarmsSettingsView.h"
#include "gfx/TrueTypeFontCache.h"
#include "utils/ViewFormatUtils.hpp"

namespace PiAlarm::view::ssd1322 {

    AlarmsSettingsView::AlarmsSettingsView(const model::AlarmsData& alarmsData, const controller::AlarmController& alarmController)
        : IView{},
        alarmsData_{alarmsData},
        alarmController_{alarmController},
        dirty_{true},
        alarmCount_{alarmsData.alarmCount()},

        // render attributes
        alarmTimeFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_Regular, 48)},
        alarmActivationFont_{gfx::TrueTypeFontCache::getFont(FONT_MozillaText_Light, 14)}
    {}

    void AlarmsSettingsView::refresh() {
        auto currentAlarmTime {alarmsData_.getAlarm(currentSelectedAlarm_).getTime()};

        if (editState_.currentEdited == AlarmEditState::Part::None) { // only refresh if not editing
            currentHour_ = currentAlarmTime.hour();
            currentMinute_ = currentAlarmTime.minute();
            currentActivation_ = alarmsData_.getAlarm(currentSelectedAlarm_).isEnabled();
        }

        dirty_ = true;
    }

    void AlarmsSettingsView::render(RenderType &renderer) const {
        auto centerX = renderer.getWidth() / 2;
        auto centerY = renderer.getHeight() / 2;
        auto colonSeparatorDimensions = renderer.drawText(
            centerX, centerY + colonSeparatorOffsetY,
            ":",
            alarmTimeFont_,
            gfx::Canvas::Anchor::Center
        );

        auto digitSpacingFromCenter = colonSeparatorDimensions.width / 2 + digitColonSpacing_;
        auto digitBaseline = centerY + colonSeparatorDimensions.height / 2;
        drawHour(renderer, centerX - digitSpacingFromCenter, digitBaseline);
        drawMinute(renderer, centerX + digitSpacingFromCenter, digitBaseline);
        drawActivation(renderer);
    }

    void AlarmsSettingsView::drawHour(const RenderType &renderer, size_t x, size_t baseline) const {
        auto dimensions = renderer.drawText(
            x, baseline,
            utils::formatInt(currentHour_, 2),
            alarmTimeFont_,
            gfx::Canvas::Anchor::BottomRight
        );

        if (editState_.currentEdited == AlarmEditState::Part::Hour) {
            // Highlight the hour part if it is being edited
            auto topLeftX = x - dimensions.width;
            auto topLeftY = baseline - dimensions.height;

            highlightContent(renderer, topLeftX, topLeftY, dimensions.width, dimensions.height);
        }
    }

    void AlarmsSettingsView::drawMinute(const RenderType &renderer, size_t x, size_t baseline) const {
        auto dimensions = renderer.drawText(
            x, baseline,
            utils::formatInt(currentMinute_, 2),
            alarmTimeFont_,
            gfx::Canvas::Anchor::BottomLeft
        );

        if (editState_.currentEdited == AlarmEditState::Part::Minute) {
            auto topLeftY = baseline - dimensions.height;

            highlightContent(renderer, x, topLeftY, dimensions.width, dimensions.height);
        }
    }

    void AlarmsSettingsView::drawActivation(const RenderType &renderer) const {
        auto centerX = renderer.getWidth() / 2;
        auto bottomY = renderer.getHeight() - borderScreenVerticalSpacing_;
        auto dimensions = renderer.drawText(
            centerX, bottomY,
            std::string("Alarme ") + std::string(currentActivation_ ? "activée" : "désactivée"),
            alarmActivationFont_,
            gfx::Canvas::Anchor::BottomCenter
        );

        if (editState_.currentEdited == AlarmEditState::Part::Activation) {
            auto topLeftX = centerX - dimensions.width / 2;
            auto topLeftY = bottomY - dimensions.height;

            highlightContent(renderer, topLeftX, topLeftY, dimensions.width, dimensions.height);
        }
    }

    void AlarmsSettingsView::highlightContent(const RenderType &renderer, size_t topLeftX, size_t topLeftY, size_t contentWidth, size_t contentHeight) const {
        auto borderAreaWidth = highlightBorderWidth_ + highlightedContentPadding_;

        renderer.drawRectangle(
            topLeftX - borderAreaWidth,
            topLeftY - borderAreaWidth,
            contentWidth + 2 * borderAreaWidth,
            contentHeight + 2 * borderAreaWidth,
            highlightBorderWidth_,
            highlightBorderColor_
        );
    }

    void AlarmsSettingsView::handleInputEvent(const input::InputEvent &event) {
        bool effectivelyHandled = false;

        switch (editState_.currentEdited) {
            case AlarmEditState::Part::None:
                effectivelyHandled = handleNoneStateInput(event);
                break;
            case AlarmEditState::Part::Hour:
                effectivelyHandled = handleHourStateInput(event);
                break;
            case AlarmEditState::Part::Minute:
                effectivelyHandled = handleMinuteStateInput(event);
                break;
            case AlarmEditState::Part::Activation:
                effectivelyHandled = handleActivationStateInput(event);
                break;
        }

        dirty_ = effectivelyHandled;
    }

    bool AlarmsSettingsView::handleNoneStateInput(const input::InputEvent &event) {
        if (!event.pressed) return false;

        switch (event.button) {
            case input::ButtonId::Main: // enter edit mode
                editState_.currentEdited = nextEditState();
                break;
            case input::ButtonId::Previous:
                currentSelectedAlarm_ = (currentSelectedAlarm_ + alarmCount_ - 1) % alarmCount_;
                break;
            case input::ButtonId::Next:
                currentSelectedAlarm_ = (currentSelectedAlarm_ + 1) % alarmCount_;
                break;
            default:
                return false; // Not handled
        }
        return true; // Input was handled
    }

    bool AlarmsSettingsView::handleHourStateInput(const input::InputEvent &event) {
        if (!event.pressed) return false;

        switch (event.button) {
            case input::ButtonId::Main: // confirm hour and move to next part
                editState_.currentEdited = nextEditState();
                break;
            case input::ButtonId::Previous: // decrement hour
                currentHour_ = (currentHour_ + 23) % 24;
                break;
            case input::ButtonId::Next: // increment hour
                currentHour_ = (currentHour_ + 1) % 24;
                break;
            default:
                return false; // Not handled
        }
        return true; // Input was handled
    }

    bool AlarmsSettingsView::handleMinuteStateInput(const input::InputEvent &event) {
        if (!event.pressed) return false;

        switch (event.button) {
            case input::ButtonId::Main: // confirm minute and move to activation state
                editState_.currentEdited = nextEditState();
                break;
            case input::ButtonId::Previous: // decrement minute
                currentMinute_ = (currentMinute_ + 59) % 60;
                break;
            case input::ButtonId::Next: // increment minute
                currentMinute_ = (currentMinute_ + 1) % 60;
                break;
            default:
                return false; // Not handled
        }
        return true; // Input was handled
    }

    bool AlarmsSettingsView::handleActivationStateInput(const input::InputEvent &event) {
        if (!event.pressed) return false;

        switch (event.button) {
            case input::ButtonId::Main: // confirm activation and exit edit mode
                alarmController_.setAlarm(currentSelectedAlarm_, currentHour_, currentMinute_, currentActivation_);
                editState_.currentEdited = nextEditState();
                break;
            case input::ButtonId::Previous:
            case input::ButtonId::Next:
                currentActivation_ = !currentActivation_; // toggle
                break;
            default:
                return false; // Not handled
        }
        return true;
    }

    AlarmsSettingsView::AlarmEditState::Part AlarmsSettingsView::nextEditState() const {
        switch (editState_.currentEdited) {
            case AlarmEditState::Part::None:       return AlarmEditState::Part::Hour;
            case AlarmEditState::Part::Hour:       return AlarmEditState::Part::Minute;
            case AlarmEditState::Part::Minute:     return AlarmEditState::Part::Activation;
            case AlarmEditState::Part::Activation: return AlarmEditState::Part::None;
        }
        return AlarmEditState::Part::None; // fallback
    }

} // namespace PiAlarm::view::ssd1322
