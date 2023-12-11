#include <algorithm>

#include "../../library/input/input.h"
#include "../../library/render/effects.h"
#include "../../library/render/render.h"
#include "title_bar.h"
#include "window.h"

gui::window::window(std::string_view text) {
    set_text(text);
}

bool gui::window::event(const base_event &ev) {
    if (ev.type() == mouse_event::mouse_move && m_dragging) {
        const auto display_size = render::get_display_size();
        const auto new_pos = input::get_mouse_pos() - m_interaction_mouse_pos + m_interaction_offset;

        m_position.x = std::clamp(new_pos.x, 2.0f, display_size.x - get_size().x * 0.25f - 2.0f);
        m_position.y = std::clamp(new_pos.y, 2.0f, display_size.y - get_size().y * 0.25f - 2.0f);

        return true;
    }

    return false;
}

void gui::window::layout(layout_node &parent) {
    if (!m_node) {
        m_node = {};
    }

    apply_properties();

    m_node.position_type(POSITION_TYPE_ABSOLUTE);
    m_node.flex_direction(FLEX_DIRECTION_COLUMN);
    m_node.justify_content(JUSTIFY_FLEX_START);
    m_node.align_items(ALIGN_FLEX_START);
    m_node.position(EDGE_LEFT, m_position.x);
    m_node.position(EDGE_TOP, m_position.y);

    for (const auto &child : get_children()) {
        if (child->get_visible()) {
            child->layout(m_node);
        }
    }

    m_node.calculate(FLT_MAX, FLT_MAX, DIRECTION_LTR);
}

void gui::window::render() {
    const auto [pos, size] = m_node.get_rect();

    // Nice hack to draw everything that's drawn under the window itself :)))
    // Makes GPU very angry!!!
    auto do_blur = false;
    for (const auto &child : get_children()) {
        if (child->is_a<title_bar>()) {
            do_blur = true;
            break;
        }
    }

    if (do_blur) {
        render::effects::apply_blur(16.0f, 4.0f, 16.0f);
    }

    // Draw window shadow
    render::draw_shadow_rect({pos.x + 8, pos.y}, {size.x - 16, size.y}, {0, 0, 0, 100}, 12.0f, 4.0f, CORNER_ALL, {0.0f, 2.0f});
    render::draw_shadow_rect({pos.x, pos.y}, {size.x, size.y}, {0, 0, 0, 64}, 8.0f, 4.0f, CORNER_ALL, {0.0f, 1.0f});

    // Draw window background
    render::fill_rect(pos, size, colors::gray_750, 4.0f);

    // Draw window border
    render::draw_rect(pos, size, colors::gray_600 / 25, 4.0f);

    // Render all children
    control::render();

    // Render the tint over the window
    render::fill_rect(pos, size, m_tint_color, 4.0f);
}
