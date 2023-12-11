#include <functional>

#include "../core/settings/settings.h"
#include "../library/console/console.h"
#include "../library/input/input.h"
#include "../library/render/render.h"
#include "../vendor/imgui/imgui.h"
#include "gui.h"

using namespace gui;

static std::vector<std::shared_ptr<window>> windows;
static std::vector<std::shared_ptr<window>> modal_windows;
static std::vector<std::shared_ptr<window>> popup_windows;

static std::shared_ptr<window> target_window;
static std::shared_ptr<window> last_clicked_window;
static std::shared_ptr<window> current_hovered_window;

static layout_node fake_root_node = nullptr;
static point_t last_mouse_pos;

static bool is_dragging;

static void draw_layout_bounds(const gui::layout_node &node, int color_index = 0) {
    static const color_t layout_border_colors[15] = {
        colors::slate_200,  colors::red_500,    colors::orange_500,  colors::amber_500, colors::yellow_500,
        colors::lime_500,   colors::green_500,  colors::teal_500,    colors::sky_500,   colors::indigo_500,
        colors::violet_500, colors::purple_500, colors::fuchsia_500, colors::pink_500,  colors::rose_500};

    render::draw_rect(node.get_position(), node.get_size(), layout_border_colors[color_index++ % 15]);

    for (size_t i = 0; i < node.child_count(); i++) {
        draw_layout_bounds(node.at(i), color_index);
    }
}

static void move_window_to_front(const std::shared_ptr<window> &window) {
    if (windows.back() != window) {
        gui::remove_window(window);
        gui::add_window(window);
    }
}

static std::shared_ptr<window> get_window_under_cursor(
    const std::vector<std::shared_ptr<window>> &windows,  //
    bool ignore_popups = false
) {
    if (!ignore_popups && !popup_windows.empty()) {
        const auto &window = popup_windows.back();

        if (input::is_mouse_in_bounds(window->get_position(), window->get_position() + window->get_size())) {
            return window;
        }

        return nullptr;
    }

    for (int i = static_cast<int>(windows.size() - 1); i >= 0; i--) {
        const auto &window = windows[i];

        if (!window->get_visible() || window->get_blocked_by()) {
            continue;
        }

        if (input::is_mouse_in_bounds(window->get_position(), window->get_position() + window->get_size())) {
            return window;
        }
    }

    return nullptr;
}

static std::shared_ptr<control> dispatch_event_impl(
    const base_event &ev,
    const std::shared_ptr<control> &target,  //
    const std::shared_ptr<control> &stay_within = nullptr
) {
    if (!target->get_visible()) {
        return nullptr;
    }

    // Dispatch the event to the target control
    if (target->event(ev)) {
        return target;
    }

    // If we're not supposed to pass events beyond this control, return
    if (target == stay_within) {
        return nullptr;
    }

    // Try dispatching the event to the children
    for (const auto &child : target->get_children()) {
        if (const auto result = dispatch_event_impl(ev, child, stay_within)) {
            return result;
        }
    }

    return nullptr;
}

static std::shared_ptr<control> dispatch_event(
    const base_event &ev,
    const std::shared_ptr<control> &target,  //
    const std::shared_ptr<control> &stay_within = nullptr
) {
    const auto result = dispatch_event_impl(ev, target, stay_within);

    if (result) {
        LOG("Event {} handled by [{} {:#x}]", ev.type(), result->get_object_name(), reinterpret_cast<uintptr_t>(result.get()));
    } else {
        LOG("Event {} was dropped (initially dispatched to [{} {:#x}])",  //
            ev.type(), target->get_object_name(), reinterpret_cast<uintptr_t>(target.get()));
    }

    return result;
}

static void handle_popout_focus_out() {
    if (popup_windows.empty() || last_clicked_window != popup_windows.back()) {
        last_clicked_window = nullptr;
        return;
    }

    const auto hovered_popup_window = get_window_under_cursor(popup_windows, true);

    while (!popup_windows.empty() && popup_windows.back() != hovered_popup_window) {
        remove_window(popup_windows.back());
    }

    if (!popup_windows.empty()) {
        last_clicked_window = popup_windows.back();
    } else {
        last_clicked_window = nullptr;
    }
}

void gui::add_window(const std::shared_ptr<window> &window) {
    windows.push_back(window);
    last_clicked_window = window;
}

void gui::add_modal_window(const std::shared_ptr<window> &parent_window, const std::shared_ptr<window> &window) {
    add_window(window);
    modal_windows.push_back(window);

    parent_window->set_blocked_by(window);
    parent_window->set_tint_color({0, 0, 0, 80});
}

void gui::add_popup_window(const std::shared_ptr<window> &window) {
    add_window(window);
    popup_windows.push_back(window);
}

void gui::remove_window(const std::shared_ptr<window> &window) {
    std::erase(windows, window);
    std::erase(modal_windows, window);
    std::erase(popup_windows, window);

    for (const auto &it : windows) {
        if (it->get_blocked_by() == window) {
            it->set_blocked_by(nullptr);
            it->set_tint_color({0, 0, 0, 0});
            break;
        }
    }
}

void gui::frame() {
    // Layout and render all windows and their children
    for (const auto &window : windows) {
        if (!window->get_visible()) {
            continue;
        }

        window->layout(fake_root_node);
        window->render();

        if (GET_BOOL("ui.draw_layout_bounds")) {
            draw_layout_bounds(window->get_node());
        }
    }

    // Process inputs and dispatch events
    const auto mouse_pos = input::get_mouse_pos();
    const auto hovered_window = get_window_under_cursor(windows);

    if (!target_window && hovered_window != current_hovered_window) {
        if (current_hovered_window) {
            dispatch_event(mouse_event{mouse_event::mouse_leave, 0, mouse_pos}, current_hovered_window);
        }

        if (hovered_window) {
            dispatch_event(mouse_event{mouse_event::mouse_hover, 0, mouse_pos}, hovered_window);
        }

        current_hovered_window = hovered_window;
    }

    if (input::is_mouse_clicked(MOUSE_LEFT)) {
        if (hovered_window) {
            if (last_clicked_window != hovered_window) {
                if (last_clicked_window) {
                    dispatch_event(window_event{window_event::focus_out}, last_clicked_window);
                    handle_popout_focus_out();
                }

                dispatch_event(window_event{window_event::focus_in}, hovered_window);
                last_clicked_window = hovered_window;
            }

            if (dispatch_event(window_event{window_event::drag_start}, hovered_window)) {
                is_dragging = true;
            } else {
                dispatch_event(mouse_event{mouse_event::mouse_click, MOUSE_LEFT, mouse_pos}, hovered_window);
                is_dragging = false;
            }

            target_window = hovered_window;
            last_mouse_pos = mouse_pos;
            move_window_to_front(target_window);
        } else {
            if (last_clicked_window) {
                dispatch_event(window_event{window_event::focus_out}, last_clicked_window);
                handle_popout_focus_out();
            }
        }
    } else if (target_window && input::is_mouse_released(MOUSE_LEFT)) {
        if (is_dragging) {
            dispatch_event(window_event{window_event::drag_end}, target_window);
        } else {
            dispatch_event(mouse_event{mouse_event::mouse_up, MOUSE_LEFT, mouse_pos}, target_window);
        }

        is_dragging = false;
        target_window = nullptr;
    } else if (mouse_pos != last_mouse_pos) {
        if (target_window) {
            dispatch_event(mouse_event{mouse_event::mouse_move, MOUSE_LEFT, mouse_pos}, target_window);
        } else if (hovered_window) {
            dispatch_event(mouse_event{mouse_event::mouse_move, MOUSE_LEFT, mouse_pos}, hovered_window);
        }

        last_mouse_pos = mouse_pos;
    }

    if (!last_clicked_window) {
        return;
    }

    auto &io = ImGui::GetIO();

    for (const auto ch : io.InputQueueCharacters) {
        const key_event event{key_event::text_char, ch, false, io.KeyCtrl, io.KeyShift, io.KeyAlt};
        dispatch_event(event, last_clicked_window);
    }

    io.InputQueueCharacters.resize(0);

    for (auto key = ImGuiKey_KeysData_OFFSET; key < ImGuiKey_COUNT; key = static_cast<ImGuiKey>(key + 1)) {
        const auto pressed = input::is_key_pressed(static_cast<key_code_t>(key));
        const auto repeated = input::is_key_pressed(static_cast<key_code_t>(key), true) && !pressed;

        if (pressed || repeated) {
            const key_event event{key_event::key_down, static_cast<uint32_t>(key), repeated, io.KeyCtrl, io.KeyShift, io.KeyAlt};
            dispatch_event(event, last_clicked_window);
        } else if (input::is_key_released(static_cast<key_code_t>(key))) {
            const key_event event{key_event::key_up, static_cast<uint32_t>(key), repeated, io.KeyCtrl, io.KeyShift, io.KeyAlt};
            dispatch_event(event, last_clicked_window);
        }
    }
}
