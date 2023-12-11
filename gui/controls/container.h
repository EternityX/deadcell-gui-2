#pragma once

#include "control.h"

namespace gui {

    class container : public control {
    private:
        flex_direction_t m_flex_direction = FLEX_DIRECTION_COLUMN;
        justify_t m_justify_content = JUSTIFY_FLEX_START;
        align_t m_align_items = ALIGN_FLEX_START;
        align_t m_align_content = ALIGN_FLEX_START;

        float m_spacing;

    public:
        void layout(layout_node &parent) override;

#ifdef _DEBUG
        std::string_view get_object_name() override {
            return "container";
        }
#endif

        flex_direction_t get_flex_direction() const {
            return m_flex_direction;
        }

        justify_t get_justify_content() const {
            return m_justify_content;
        }

        align_t get_align_items() const {
            return m_align_items;
        }

        align_t get_align_content() const {
            return m_align_content;
        }

        float get_spacing() const {
            return m_spacing;
        }

        void set_flex_direction(flex_direction_t value) {
            m_flex_direction = value;
        }

        void set_justify_content(justify_t value) {
            m_justify_content = value;
        }

        void set_align_items(align_t value) {
            m_align_items = value;
        }

        void set_align_content(align_t value) {
            m_align_content = value;
        }

        void set_spacing(float value) {
            m_spacing = value;
        }
    };

}  // namespace gui
