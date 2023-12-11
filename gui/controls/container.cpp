#include "container.h"

void gui::container::layout(layout_node &parent) {
    control::layout(parent);

    m_node.flex_direction(m_flex_direction);
    m_node.justify_content(m_justify_content);
    m_node.align_items(m_align_items);
    m_node.align_content(m_align_content);

    const auto &children = get_children();

    for (size_t i = 0; i < children.size(); i++) {
        const auto &child = children[i];

        if (i > 0) {
            const auto x_margin = m_flex_direction == FLEX_DIRECTION_ROW ? m_spacing : 0.0f;
            const auto y_margin = m_flex_direction == FLEX_DIRECTION_COLUMN ? m_spacing : 0.0f;
            child->set_margin_tl({x_margin, y_margin});
        }

        child->layout(m_node);
    }
}
