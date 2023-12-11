#include "control.h"

#include <format>

void gui::control::apply_properties() {
    if (!m_props_dirty) {
        return;
    }

    if (std::isnan(m_size.x)) {
        m_node.width(_auto);
    } else if (m_size.x < 0.0f) {
        m_node.width(100.0_pct);
    } else {
        m_node.width(m_size.x);
    }

    if (std::isnan(m_size.y)) {
        m_node.height(_auto);
    } else if (m_size.y < 0.0f) {
        m_node.height(100.0_pct);
    } else {
        m_node.height(m_size.y);
    }

    if (!std::isnan(m_min_size.x)) {
        if (m_min_size.x < 0.0f) {
            m_node.min_width(100.0_pct);
        } else {
            m_node.min_width(m_min_size.x);
        }
    }

    if (!std::isnan(m_min_size.y)) {
        if (m_min_size.y < 0.0f) {
            m_node.min_height(100.0_pct);
        } else {
            m_node.min_height(m_min_size.y);
        }
    }

    if (!std::isnan(m_max_size.x)) {
        if (m_max_size.x < 0.0f) {
            m_node.max_width(100.0_pct);
        } else {
            m_node.max_width(m_max_size.x);
        }
    }

    if (!std::isnan(m_max_size.y)) {
        if (m_max_size.y < 0.0f) {
            m_node.max_height(100.0_pct);
        } else {
            m_node.max_height(m_max_size.y);
        }
    }

    m_node.flex_grow(m_flex_grow);
    m_node.flex_shrink(m_flex_shrink);
    m_node.align_self(m_align_self);

#define HANDLE_PADDING(edge, value)      \
    if (value < 0.0f) {                  \
        m_node.padding(edge, 100.0_pct); \
    } else if (value > 0.0f) {           \
        m_node.padding(edge, value);     \
    }

#define HANDLE_MARGIN(edge, value)      \
    if (std::isnan(value)) {            \
        m_node.margin(edge, _auto);     \
    } else if (value < 0.0f) {          \
        m_node.margin(edge, 100.0_pct); \
    } else {                            \
        m_node.margin(edge, value);     \
    }

    HANDLE_PADDING(EDGE_LEFT, m_padding_tl.x)
    HANDLE_PADDING(EDGE_RIGHT, m_padding_br.x)
    HANDLE_PADDING(EDGE_TOP, m_padding_tl.y)
    HANDLE_PADDING(EDGE_BOTTOM, m_padding_br.y)

    HANDLE_MARGIN(EDGE_LEFT, m_margin_tl.x)
    HANDLE_MARGIN(EDGE_RIGHT, m_margin_br.x)
    HANDLE_MARGIN(EDGE_TOP, m_margin_tl.y)
    HANDLE_MARGIN(EDGE_BOTTOM, m_margin_br.y)
}

gui::control::~control() {
    remove_from_parent();
    remove_all_children();
}

void gui::control::add_child(const std::shared_ptr<control> &child) {
    child->m_parent = shared_from_this();

    m_children.emplace_back(child);
}

void gui::control::remove_child(const std::shared_ptr<control> &control) {
    control->m_parent.reset();

    std::erase(m_children, control);
}

void gui::control::remove_all_children() {
    while (!m_children.empty()) {
        remove_child(m_children.front());
    }
}

void gui::control::remove_from_parent() {
    if (m_parent) {
        m_parent->remove_child(shared_from_this());
    }
}

bool gui::control::is_child(const std::shared_ptr<control> &control) const {
    const auto it = std::ranges::find_if(m_children, [control](const auto &child) {
        return child == control;
    });

    return it != m_children.end();
}

bool gui::control::is_ancestor(const std::shared_ptr<control> &control) const {
    if (control == shared_from_this()) {
        return false;
    }

    for (auto it = m_parent; it; it = it->m_parent) {
        if (it == shared_from_this()) {
            return true;
        }
    }

    return false;
}
