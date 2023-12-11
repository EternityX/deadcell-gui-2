#include "watermark.h"
#include "../../library/input/input.h"
#include "../../library/render/render.h"

std::shared_ptr<gui::window> gui::watermark::get_parent_window() const {
    if (const auto parent = get_parent()) {
        return std::dynamic_pointer_cast<gui::window>(parent);
    }

    return {};
}

gui::watermark::watermark(std::string_view text, std::string_view ver_text) {
    set_size({-1.0f, 32.0f});
    set_flex_shrink(0.0f);

    m_text = text;
    m_ver_text = ver_text;
}

bool gui::watermark::event(const gui::base_event &ev) {
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

void gui::watermark::layout(gui::layout_node &parent) {
    const auto parent_window = get_parent_window();
    const auto text_size = render::get_text_size(m_text, render::menu_font_semibold);
    const auto ver_text_size = render::get_text_size(m_ver_text, render::menu_font_semibold);

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

    if (!m_ver_text_node) {
        m_ver_text_node = m_node.new_child();
    }

    m_ver_text_node.node_type(NODE_TYPE_TEXT);
    m_ver_text_node.width(ver_text_size.x);
    m_ver_text_node.height(ver_text_size.y);
    m_ver_text_node.margin(EDGE_END, 16.0f);
    m_ver_text_node.margin(EDGE_START, _auto);
    m_ver_text_node.align_self(ALIGN_CENTER);
}

void gui::watermark::render() {
    const auto [pos, size] = m_node.get_rect();
    const auto [text_pos, text_size] = m_text_node.get_rect();
    const auto [ver_text_pos, _] = m_ver_text_node.get_rect();

    render::fill_rect(pos + point_t{-2.0f, 0.0f}, {32.0f, 32.0f}, colors::accent_light, 1.0f, CORNER_ALL);
    render::fill_rect(pos + size - point_t{32.0f, 32.0f}, {34.0f, 32.0f}, colors::accent_light, 1.0f, CORNER_ALL);
    render::fill_rect(pos, size, colors::gray_750, 2.0f, CORNER_ALL);

    render::draw_shadow_rect(
        {pos.x + 8.0f, pos.y + 2.0f}, {size.x - 16.0f, size.y}, {0, 0, 0, 100}, 12.0f, 4.0f, CORNER_ALL, {0.0f, 2.0f}
    );
    render::draw_shadow_rect({pos.x, pos.y + 2.0f}, {size.x, size.y}, {0, 0, 0, 64}, 8.0f, 4.0f, CORNER_ALL, {0.0f, 1.0f});

    const auto dots_size = render::get_svg_size(render::svg_dots, 0.10f);
    const auto dots_x = static_cast<int>(std::ceil(size.x / dots_size.x));
    const auto dots_y = static_cast<int>(std::ceil(size.y / dots_size.y));

    render::push_clip({pos.x - 8.0f, pos.y}, {pos.x + size.x + 8.0f, pos.y + size.y});

    for (int y = 0; y < dots_y; y++) {
        for (int x = 0; x < dots_x; x++) {
            render::draw_svg(
                {pos.x + dots_size.x * x, pos.y + dots_size.y * y},  //
                render::svg_dots, {255, 255, 255}, 0.10f
            );
        }
    }

    render::pop_clip();

    render::draw_text(text_pos, m_text, render::menu_font_semibold, colors::slate_300);
    render::draw_text(ver_text_pos, m_ver_text, render::menu_font_semibold, colors::accent_light);
}
