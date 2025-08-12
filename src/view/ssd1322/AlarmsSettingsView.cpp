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

        currentHour_ = currentAlarmTime.hour();
        currentMinute_ = currentAlarmTime.minute();
        currentActivation_ = alarmsData_.getAlarm(currentSelectedAlarm_).isEnabled();

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
        drawActivation(renderer, digitBaseline + digitActivationSpacing_);
    }

    void AlarmsSettingsView::drawHour(RenderType &renderer, size_t x, size_t baseline) const {
        auto dimensions = renderer.drawText(
            x, baseline,
            utils::formatInt(currentHour_, 2),
            alarmTimeFont_,
            gfx::Canvas::Anchor::BottomRight
        );
    }

    void AlarmsSettingsView::drawMinute(RenderType &renderer, size_t x, size_t baseline) const {
        auto dimensions = renderer.drawText(
            x, baseline,
            utils::formatInt(currentMinute_, 2),
            alarmTimeFont_,
            gfx::Canvas::Anchor::BottomLeft
        );
    }

    void AlarmsSettingsView::drawActivation(RenderType &renderer, size_t topY) const {
        auto dimensions = renderer.drawText(
            renderer.getWidth()/2, topY,
            std::string("Alarme ") + std::string(currentActivation_ ? "activée" : "désactivée"),
            alarmActivationFont_,
            gfx::Canvas::Anchor::TopCenter
        );
    }

    void AlarmsSettingsView::handleInputEvent(const input::InputEvent &event) {

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
