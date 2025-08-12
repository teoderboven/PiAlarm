#ifndef PIALARM_ALARMSSETTINGSVIEW_H
#define PIALARM_ALARMSSETTINGSVIEW_H

#include "view/IView.h"
#include "model/AlarmsData.hpp"
#include "controller/AlarmController.h"

namespace PiAlarm::view::ssd1322 {

    /**
     * @brief AlarmsSettingsView class for managing and displaying alarm settings.
     * This class is responsible for rendering the alarms settings view and handling user input
     * to modify alarm settings such as time and activation status.
     */
    class AlarmsSettingsView : public IView {

        /**
         * @brief Represents the state of the alarm edit operation.
         * This struct is used to track which part of the alarm is currently being edited (hour, minute, activation).
         */
        struct AlarmEditState {

            /**
             * @brief Enum class representing the part of the alarm being edited.
             * This enum defines the different parts of an alarm that can be modified.
             */
            enum class Part : uint8_t {
                None,      ///< No part is being edited
                Hour,      ///< The hour part of the alarm is being edited
                Minute,    ///< The minute part of the alarm is being edited
                Activation ///< The activation status of the alarm is being edited
            };

            Part currentEdited = Part::None; ///< Current part being edited
        };

        const model::AlarmsData& alarmsData_; ///< Reference to the alarms data model
        const controller::AlarmController& alarmController_; ///< Reference to the alarm controller

        // No need for dirty to be atomic here, as it is only accessed from the main thread (no update from observer)
        bool dirty_; ///< Flag indicating if the view is dirty (needs to be refreshed)

        const size_t alarmCount_; ///< Number of alarms currently managed by the view

        AlarmEditState editState_; ///< Current state of the alarm being edited
        int currentHour_ {0}; ///< Current hour being edited
        int currentMinute_ {0}; ///< Current minute being edited
        bool currentActivation_ {false}; ///< Current activation status of the alarm being edited
        size_t currentSelectedAlarm_ {0}; ///< Index of the currently selected alarm for editing

        // render attributes

        const std::shared_ptr<gfx::IFont> alarmTimeFont_;       ///< Font used for rendering alarm times
        const std::shared_ptr<gfx::IFont> alarmActivationFont_; ///< Font used for rendering alarm activation status

        const ssize_t borderScreenVerticalSpacing_ {3};         ///< Vertical spacing for the alarm settings from the screen border
        const ssize_t colonSeparatorOffsetY {-3};               ///< Offset for the colon separator from the center Y position
        const ssize_t digitColonSpacing_ {2};                   ///< Spacing between hour/minute digits and the colon separator
        const ssize_t digitActivationSpacing_ {5};              ///< Spacing between the hour/minute digits and the activation status
        const ssize_t highlightedContentPadding_ {2};           ///< Padding around the highlighted content
        const size_t highlightBorderWidth_ {1};                 ///< Width of the highlight border around the edited part
        const gfx::Pixel highlightBorderColor_ {0x30};          ///< Color of the highlight border

    public:

        /**
         * @brief Constructs an AlarmsSettings view.
         * @param alarmsData Reference to the model::AlarmsData instance to manage.
         * @param alarmController Reference to the controller::AlarmController instance that manages alarms.
         */
        AlarmsSettingsView(const model::AlarmsData& alarmsData, const controller::AlarmController& alarmController);

        /**
         * @brief Refreshes the view by updating the alarms vector.
         * This method retrieves the current state of alarms from the model and updates the view accordingly.
         */
        void refresh() override;

        /**
         * @brief Renders the alarms settings view.
         * This method draws the current state of the alarms on the display.
         * @param renderer The renderer to use for drawing the view.
         */
        void render(RenderType &renderer) const override;

        /**
         * @brief Returns the type of the view.
         * @return The type of the view as a ViewType enum value.
         */
        [[nodiscard]]
        inline bool isDirty() const override;

        /**
         * @brief Clears the dirty state of the view.
         * This method sets the dirty flag to false, indicating that the view does not need to be refreshed.
         */
        inline void clearDirty() override;

        /**
         * @brief Handles input events for the alarms settings view.
         * This method processes user input to modify alarm settings such as time and activation status.
         * @param event The input event to handle.
         */
        void handleInputEvent(const input::InputEvent &event) override;

    private:

        /**
         * @brief Handles input events when no part of the alarm is being edited.
         * This method allows the user to select an alarm and enter edit mode.
         * @param event The input event to handle.
         * @return True if the input was handled, false otherwise.
         */
        bool handleNoneStateInput(const input::InputEvent &event);

        /**
         * @brief Handles input events when the hour part of the alarm is being edited.
         * This method allows the user to increment or decrement the hour and confirm the change.
         * @param event The input event to handle.
         * @return True if the input was handled, false otherwise.
         */
        bool handleHourStateInput(const input::InputEvent &event);

        /**
         * @brief Handles input events when the minute part of the alarm is being edited.
         * This method allows the user to increment or decrement the minute and confirm the change.
         * @param event The input event to handle.
         * @return True if the input was handled, false otherwise.
         */
        bool handleMinuteStateInput(const input::InputEvent &event);

        /**
         * @brief Handles input events when the activation state of the alarm is being edited.
         * This method allows the user to toggle the activation status and confirm the change.
         * @param event The input event to handle.
         * @return True if the input was handled, false otherwise.
         */
        bool handleActivationStateInput(const input::InputEvent &event);

        /**
         * @brief Draws the current alarm settings on the display.
         * This method renders the selected alarm's time and activation status.
         * @param renderer The renderer to use for drawing.
         * @param x The X coordinate for rendering the alarm settings.
         * @param baseline The baseline Y coordinate for rendering the alarm settings.
         */
        void drawHour(const RenderType &renderer, size_t x, size_t baseline) const;

        /**
         * @brief Draws the current minute settings on the display.
         * This method renders the selected alarm's minute part.
         * @param renderer The renderer to use for drawing.
         * @param x The X coordinate for rendering the minute settings.
         * @param baseline The baseline Y coordinate for rendering the minute settings.
         */
        void drawMinute(const RenderType &renderer, size_t x, size_t baseline) const;

        /**
         * @brief Draws the activation status of the current alarm.
         * This method renders whether the alarm is active or inactive.
         * @param renderer The renderer to use for drawing.
         */
        void drawActivation(const RenderType &renderer) const;

        /**
          * @brief Highlights the content of the current alarm part being edited.
          * This method draws a highlight around the currently edited part of the alarm settings.
          * @param renderer The renderer to use for drawing the highlight.
          * @param topLeftX The X coordinate of the top-left corner of the highlight.
          * @param topLeftY The Y coordinate of the top-left corner of the highlight.
          * @param contentWidth The width of the content to highlight.
          * @param contentHeight The height of the content to highlight.
          * @note This method is used to visually indicate which part of the alarm settings is currently being edited.
          */
        void highlightContent(const RenderType &renderer, size_t topLeftX, size_t topLeftY, size_t contentWidth, size_t contentHeight) const;

        /**
         * @brief Determines the next edit state based on the current state.
         * This method transitions to the next part of the alarm that can be edited.
         * @return The next AlarmEditState::Part to be edited.
         * @note Does not modify the current state directly, but returns the next state.
         */
        [[nodiscard]]
        AlarmEditState::Part nextEditState() const;

    };

    // Inline methods implementation

    inline bool AlarmsSettingsView::isDirty() const {
        return dirty_;
    }

    inline void AlarmsSettingsView::clearDirty() {
        dirty_ = false;
    }

} // namespace PiAlarm::view::ssd1322

#endif //PIALARM_ALARMSSETTINGSVIEW_H
