#include "group_box.h"
#include "../../library/render/render.h"

gui::group_box::group_box() {
    set_size(auto_value);
}

gui::group_box::group_box(std::string_view name) : group_box() {
    set_text(name);
}

void gui::group_box::layout(layout_node &parent) {
    control::layout(parent);

    m_node.flex_direction(FLEX_DIRECTION_COLUMN);
    m_node.justify_content(JUSTIFY_FLEX_START);

    if (!m_text.empty()) {
        if (!m_header_node) {
            m_header_node = {};
            m_node.push_front(m_header_node);
        }

        m_header_node.width(100.0_pct);

        if (!m_text_node) {
            m_text_node = m_header_node.new_child();
        }

        const auto text_size = render::get_text_size(m_text, render::menu_font_small_semibold);

        m_text_node.node_type(NODE_TYPE_TEXT);
        m_text_node.width(text_size.x);
        m_text_node.height(text_size.y);
        m_text_node.margin(EDGE_HORIZONTAL, 16.0f);
        m_text_node.margin(EDGE_VERTICAL, 8.0f);
    }

    if (!m_inner_node) {
        m_inner_node = m_node.new_child();
    }

    m_inner_node.flex_direction(FLEX_DIRECTION_COLUMN);
    m_inner_node.justify_content(JUSTIFY_FLEX_START);
    m_inner_node.align_items(ALIGN_FLEX_START);
    m_inner_node.width(100.0_pct);
    m_inner_node.height(_auto);

    m_inner_node.padding(EDGE_HORIZONTAL, 16.0f);
    m_inner_node.padding(EDGE_VERTICAL, m_text.empty() ? 16.0f : 12.0f);

    for (const auto &child : get_children()) {
        child->layout(m_inner_node);
    }
}

void gui::group_box::render() {
    const auto [pos, size] = m_node.get_rect();

    // base
    render::fill_rect(pos, size, colors::gray_800 / 50, 4.0f);

    // header
    if (m_header_node) {
        const auto [header_pos, header_size] = m_header_node.get_rect();
        const auto [text_pos, _] = m_text_node.get_rect();

        render::fill_rect(header_pos, header_size, colors::gray_800, 4.0f, CORNER_TOP);
        render::draw_text(text_pos, m_text, render::menu_font_small_semibold, colors::slate_400);
    }

    // border
    render::draw_rect(pos, size, colors::gray_600 / 25, 4.0f);

    // children
    control::render();
}
