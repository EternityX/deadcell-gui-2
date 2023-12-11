#pragma once

#define STB_TEXTEDIT_CHARTYPE wchar_t
#define STB_TEXTEDIT_POSITIONTYPE int32_t
#define STB_TEXTEDIT_UNDOSTATECOUNT 128
#define STB_TEXTEDIT_UNDOCHARCOUNT 1024
#define STB_TEXTEDIT_NEWLINE '\n'

#include <cstdint>
#include <string>

#include <stb_textedit.h>

#include "../../library/math/animation.h"
#include "../../library/render/render.h"
#include "control.h"

namespace gui {

    class text_input : public control {
    public:
        struct state_t {
            std::string *m_value;
            draw_font_t m_font;
            int m_font_size;
            int m_max_length;
        };

    protected:
        STB_TexteditState m_stb_state;
        state_t m_state;

    private:
        std::string m_placeholder;

        bool m_active;
        bool m_enabled;
        bool m_masked;
        bool m_dragging;

        float m_last_input;

    public:
        explicit text_input(std::string *value);
        explicit text_input(std::string_view placeholder, std::string *value);

        bool event(const base_event &ev) override;
        void layout(layout_node &parent) override;
        void render() override;

#ifdef _DEBUG
        std::string_view get_object_name() override {
            return "text_input";
        }
#endif

        draw_font_t get_font() const {
            return m_state.m_font;
        }

        int get_font_size() const {
            return m_state.m_font_size;
        }

        int get_max_length() const {
            return m_state.m_max_length;
        }

        std::string_view get_placeholder() const {
            return m_placeholder;
        }

        bool get_enabled() const {
            return m_enabled;
        }

        bool get_masked() const {
            return m_masked;
        }

        void set_font(draw_font_t value) {
            m_state.m_font = value;
        }

        void set_font_size(int value) {
            m_state.m_font_size = value;
        }

        void set_max_length(int value) {
            m_state.m_max_length = value;
        }

        void set_placeholder(std::string_view value) {
            m_placeholder = value;
        }

        void set_enabled(bool value) {
            m_enabled = value;
        }

        void set_masked(bool value) {
            m_masked = value;
        }
    };

}  // namespace gui
