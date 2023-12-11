#include "label.h"
#include "../../library/render/render.h"

gui::label::label(std::string_view text) {
    set_text(text);
    set_margin_br({0.0f, 10.0f});
}

void gui::label::layout(layout_node &parent) {
    const auto text_size = render::get_text_size(m_text, render::menu_font_regular);

    control::layout(parent);

    m_node.node_type(NODE_TYPE_TEXT);
    m_node.width(text_size.x);
    m_node.height(text_size.y);
}

void gui::label::render() {
    render::draw_text(m_node.get_position(), m_text, render::menu_font_regular, colors::slate_400);
}
