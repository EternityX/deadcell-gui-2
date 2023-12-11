#include "title_bar.h"
#include "../../library/input/input.h"
#include "../../library/render/effects.h"
#include "../../library/render/render.h"

std::shared_ptr<gui::window> gui::title_bar::get_parent_window() const {
    if (const auto parent = get_parent()) {
        return std::dynamic_pointer_cast<gui::window>(parent);
    }

    return {};
}

gui::title_bar::title_bar() {
    set_size({-1.0f, 46.0f});
    set_flex_shrink(0.0f);
}

bool gui::title_bar::event(const base_event &ev) {
    const auto [pos, size] = m_node.get_rect();
    if (ev.type() != window_event::drag_start && ev.type() != window_event::drag_end) {
        return false;
    }

    const auto parent_window = get_parent_window();
    if (ev.type() == window_event::drag_start && input::is_mouse_in_bounds(pos, pos + size)) {
        parent_window->set_dragging(true);
        parent_window->set_interaction_mouse_pos(input::get_mouse_pos());
        parent_window->set_interaction_offset(pos);
        return true;
    } else if (ev.type() == window_event::drag_end && parent_window->get_dragging()) {
        parent_window->set_dragging(false);
        return true;
    }

    return false;
}

void gui::title_bar::layout(layout_node &parent) {
    const auto parent_window = get_parent_window();
    const auto text_size = render::get_text_size(parent_window->get_text(), render::menu_font_large_semibold);

    control::layout(parent);

    m_node.flex_direction(FLEX_DIRECTION_ROW);
    m_node.justify_content(JUSTIFY_SPACE_BETWEEN);

    if (!m_text_node) {
        m_text_node = m_node.new_child();
    }

    m_text_node.node_type(NODE_TYPE_TEXT);
    m_text_node.width(text_size.x);
    m_text_node.height(text_size.y);
    m_text_node.margin(EDGE_START, 16.0f);
    m_text_node.margin(EDGE_END, _auto);
    m_text_node.align_self(ALIGN_CENTER);
}

void gui::title_bar::render() {
    const auto [pos, size] = m_node.get_rect();
    const auto [text_pos, _] = m_text_node.get_rect();

    const auto display_size = render::get_display_size();
    const auto start_uv = pos / display_size;
    const auto end_uv = (pos + size) / display_size;

    render::effects::draw_to_screen_uv(pos, size, start_uv, end_uv, {255, 255, 255}, 5.0f, CORNER_TOP);
    render::fill_rect(pos, size, colors::gray_800 / 85, 4.0f, CORNER_TOP);
    render::draw_text(text_pos, get_parent_window()->get_text(), render::menu_font_large_semibold, colors::slate_300);
}
