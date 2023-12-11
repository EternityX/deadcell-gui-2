#pragma once

#include <cassert>
#include <memory>

#include "../library/math/point.h"

namespace gui {

    class control;

    class base_event {
    protected:
        int m_type;

    public:
        base_event() = default;
        base_event(int type) : m_type(type) {}

        int type() const {
            return m_type;
        }
    };

    class mouse_event : public base_event {
    public:
        enum event_type {
            mouse_click = 0,
            mouse_up,
            mouse_move,
            mouse_hover,
            mouse_leave,
        };

    protected:
        int m_button;
        point_t m_position;

    public:
        mouse_event(int type, int button, point_t position) : base_event(type), m_button(button), m_position(position) {}

        int button() const {
            return m_button;
        }

        point_t position() const {
            return m_position;
        }
    };

    class key_event : public base_event {
    public:
        enum event_type {
            text_char = 0x20,
            key_down,
            key_up,
        };

    protected:
        uint32_t m_key;
        bool m_repeated;
        bool m_ctrl;
        bool m_shift;
        bool m_alt;

    public:
        key_event(int type, uint32_t key, bool repeated) : base_event(type), m_key(key), m_repeated(repeated) {}
        key_event(int type, uint32_t key, bool repeated, bool ctrl, bool shift, bool alt)
            : base_event(type), m_key(key), m_repeated(repeated), m_ctrl(ctrl), m_shift(shift), m_alt(alt) {}

        uint32_t key() const {
            return m_key;
        }

        bool repeated() const {
            return m_repeated;
        }

        bool ctrl() const {
            return m_ctrl;
        }

        bool shift() const {
            return m_shift;
        }

        bool alt() const {
            return m_alt;
        }
    };

    class window_event : public base_event {
    public:
        enum window_event_type {
            drag_start = 0x40,
            drag_end,
            focus_in,
            focus_out,
        };

        using base_event::base_event;
    };

}  // namespace gui
