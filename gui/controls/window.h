#pragma once

#include "control.h"

namespace gui {

    class window : public control {
    private:
        std::string m_text;

        bool m_dragging = false;

        point_t m_position;
        point_t m_interaction_mouse_pos;
        point_t m_interaction_offset;

        color_t m_tint_color = {0, 0, 0, 0};
        std::shared_ptr<window> m_blocked_by;

    public:
        explicit window(std::string_view text);

        bool event(const base_event &ev) override;
        void layout(layout_node &parent) override;
        void render() override;

#ifdef _DEBUG
        std::string_view get_object_name() override {
            return "window";
        }
#endif

        std::string_view get_text() const {
            return m_text;
        }

        bool get_dragging() const {
            return m_dragging;
        }

        point_t get_position() const {
            return m_position;
        }

        point_t get_interaction_mouse_pos() const {
            return m_interaction_mouse_pos;
        }

        point_t get_interaction_offset() const {
            return m_interaction_offset;
        }

        color_t get_tint_color() const {
            return m_tint_color;
        }

        const std::shared_ptr<window> &get_blocked_by() const {
            return m_blocked_by;
        }

        void set_text(std::string_view value) {
            m_text = value;
        }

        void set_dragging(bool value) {
            m_dragging = value;
        }

        void set_position(point_t value) {
            m_position = value;
        }

        void set_interaction_mouse_pos(point_t value) {
            m_interaction_mouse_pos = value;
        }

        void set_interaction_offset(point_t value) {
            m_interaction_offset = value;
        }

        void set_tint_color(color_t value) {
            m_tint_color = value;
        }

        void set_blocked_by(const std::shared_ptr<window> &window) {
            m_blocked_by = window;
        }
    };

}  // namespace gui
