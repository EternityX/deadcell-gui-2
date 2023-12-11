#include "checkbox.h"
#include "../../library/input/input.h"
#include "../../library/render/render.h"
#include "../../vendor/fonts/font_fa_defines.h"

gui::checkbox::checkbox(std::string_view text, bool *var, const std::function<void(const std::shared_ptr<control> &)> &callback) {
    if (!var) {
        this->remove_from_parent();
    }

    set_text(text);
    set_enabled(true);
    set_margin_br({0.0f, 10.0f});
    set_size(auto_value);

    m_checked = var;
    m_callback_func = std::move(callback);
}

bool gui::checkbox::event(const gui::base_event &ev) {
    auto handled = false;
    if (ev.type() == mouse_event::mouse_move && m_enabled) {
        const auto [pos, size] = m_node.get_rect();
        const auto is_hovered = input::is_mouse_in_bounds(pos, pos + size);

        if (m_hovered != is_hovered && !m_clicked) {
            m_hovered_anim.start(math::EASE_OUT_SINE, 0.3f, m_hovered ? 1.0f : 0.0f, is_hovered ? 1.0f : 0.0f);
        }

        handled = m_hovered = is_hovered;
    } else if (ev.type() == window_event::focus_out && m_hovered) {
        if (m_hovered) {
            m_hovered_anim.start(math::EASE_OUT_SINE, 0.3f, m_hovered ? 1.0f : 0.0f, 0.0f);
        }

        m_hovered = false;
    } else if (ev.type() == mouse_event::mouse_click && m_hovered && m_enabled) {
        // Mouse was clicked, we only want to call the callback on mouse up
        handled = m_clicked = true;
    } else if (ev.type() == mouse_event::mouse_up) {
        const auto [pos, size] = m_node.get_rect();
        m_hovered = input::is_mouse_in_bounds(pos, pos + size);

        // Mouse was clicked, call callback
        if (m_hovered && m_enabled && m_clicked) {
            *m_checked ^= true;

            if (m_callback_func) {
                m_callback_func(shared_from_this());
            }

            if (*m_checked) {
                m_toggled_anim.start(math::EASE_OUT_SINE, 0.15f, 0.0f, 1.0f);
            } else {
                m_toggled_anim.start(math::EASE_OUT_SINE, 0.15f, 1.0f, 0.0f);
            }

            handled = true;
        }

        if (m_clicked && !m_hovered) {
            m_hovered_anim.start(math::EASE_OUT_SINE, 0.15f, 1.0f, 0.0f);
        }

        m_clicked = false;
    }

    return handled;
}

void gui::checkbox::layout(layout_node &parent) {
    const auto text_size = render::get_text_size(m_text, render::menu_font_regular);

    control::layout(parent);

    m_node.flex_direction(FLEX_DIRECTION_ROW);

    if (!m_check_node) {
        m_check_node = m_node.new_child();
    }

    m_check_node.width(16.0f);
    m_check_node.height(16.0f);

    if (!m_label_node) {
        m_label_node = m_node.new_child();
    }

    m_label_node.flex_direction(FLEX_DIRECTION_COLUMN);
    m_label_node.margin(EDGE_START, 10.0f);

    if (!m_text_node) {
        m_text_node = m_label_node.new_child();
    }

    m_text_node.width(text_size.x);
    m_text_node.height(text_size.y);

    if (!m_sub_text.empty()) {
        const auto sub_text_size = render::get_text_size(m_sub_text, render::menu_font_small_semibold);

        if (!m_sub_text_node) {
            m_sub_text_node = m_label_node.new_child();
        }

        m_sub_text_node.width(sub_text_size.x);
        m_sub_text_node.height(sub_text_size.y);
    }
}

void gui::checkbox::render() {
    const auto [pos, size] = m_check_node.get_rect();
    const auto [text_pos, _] = m_text_node.get_rect();

    // label
    render::draw_text(text_pos, m_text, render::menu_font_regular, colors::slate_400);

    if (m_sub_text_node) {
        const auto [sub_text_pos, _] = m_sub_text_node.get_rect();

        render::draw_text(sub_text_pos, m_sub_text, render::menu_font_small_semibold, colors::slate_500);
    }

    // base
    render::fill_rect(pos, size, colors::gray_700, 2.0f);

    // hover
    render::fill_rect(pos, size, colors::gray_600 / (m_hovered_anim.get() * 0.25f), 2.0f);

    // border
    render::draw_rect(pos, size, colors::accent_border, 2.0f);

    if (const auto toggle_anim = m_toggled_anim.get(); toggle_anim > 0.01f) {
        const auto check_size = render::get_text_size(ICON_FA_CHECK, render::menu_font_icon, 0.0f, 10);
        const auto check_pos = pos + size * 0.5f - check_size * 0.5f;

        render::fill_rect(pos, size, colors::accent / toggle_anim, 2.0f);

        render::push_clip(check_pos, {check_pos.x + check_size.x * toggle_anim, check_pos.y + check_size.y});
        render::draw_text(check_pos, ICON_FA_CHECK, render::menu_font_icon, colors::gray_800, 0.0f, 10);
        render::pop_clip();
    }
}
