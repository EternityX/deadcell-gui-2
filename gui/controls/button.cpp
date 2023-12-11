#include "button.h"
#include "../../library/input/input.h"
#include "../../library/render/render.h"
#include "../../vendor/fonts/font_fa_defines.h"

gui::button::button(std::string_view text, const std::function<void(const std::shared_ptr<control> &)> &callback)
    : button(text, {}, callback) {}

gui::button::button(
    std::string_view text,
    std::string_view icon,
    const std::function<void(const std::shared_ptr<control> &)> &callback
) {
    set_text(text);
    set_icon_text(icon);
    set_enabled(true);
    set_margin_br({0.0f, 10.0f});
    set_size(auto_value);

    m_callback_func = std::move(callback);
}

bool gui::button::event(const gui::base_event &ev) {
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
            if (m_callback_func) {
                m_callback_func(shared_from_this());
            }

            handled = true;
        }

        if (m_clicked && !m_hovered) {
            m_hovered_anim.start(math::EASE_OUT_SINE, 0.3f, 1.0f, 0.0f);
        }

        m_clicked = false;
    }

    return handled;
}

void gui::button::layout(layout_node &parent) {
    const auto text_size = render::get_text_size(m_text, render::menu_font_small_semibold);

    control::layout(parent);

    m_node.justify_content(JUSTIFY_CENTER);
    m_node.align_items(ALIGN_CENTER);

    if (!m_inner_node) {
        m_inner_node = m_node.new_child();
    }

    m_inner_node.flex_direction(FLEX_DIRECTION_ROW);
    m_inner_node.margin(EDGE_HORIZONTAL, 16.0f);
    m_inner_node.margin(EDGE_VERTICAL, 10.0f);

    if (!m_icon.empty()) {
        const auto icon_size = render::get_text_size(m_icon, render::menu_font_icon);

        if (!m_icon_node) {
            m_icon_node = m_inner_node.new_child();
        }

        m_icon_node.width(icon_size.x);
        m_icon_node.height(icon_size.y);
    }

    if (!m_text_node) {
        m_text_node = m_inner_node.new_child();
    }

    m_text_node.node_type(NODE_TYPE_TEXT);
    m_text_node.width(text_size.x);
    m_text_node.height(text_size.y);
    m_text_node.margin(EDGE_START, m_icon.empty() ? 0.0f : 8.0f);
}

void gui::button::render() {
    const auto [pos, size] = m_node.get_rect();
    const auto [text_pos, _] = m_text_node.get_rect();

    // base
    render::fill_rect(pos, size, colors::gray_700, 6.0f);

    // hover
    render::fill_rect(pos, size, colors::gray_600 / (m_hovered_anim.get() * 0.25f), 2.0f);

    // border
    render::draw_rect(pos, size, colors::accent_border, 6.0f);

    // label
    render::draw_text(text_pos, m_text, render::menu_font_small_semibold, colors::slate_300);

    // icon
    if (m_icon_node) {
        const auto [icon_pos, _] = m_icon_node.get_rect();

        render::draw_text(icon_pos, m_icon, render::menu_font_icon, colors::slate_300);
    }

    // shadow the hedgehog
    render::draw_shadow_rect(pos, size, {0, 0, 0, 20}, 5.0f, 6.0f, CORNER_ALL, {0.0f, 1.0f});
}
