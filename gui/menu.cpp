#include "menu.h"
#include "../../core/settings/settings.h"
#include "../../gui/controls/button.h"
#include "../../gui/controls/checkbox.h"
#include "../../gui/controls/container.h"
#include "../../gui/controls/group_box.h"
#include "../../gui/controls/label.h"
#include "../../gui/controls/text_input.h"
#include "../../gui/controls/title_bar.h"
#include "../../gui/controls/watermark.h"
#include "../../gui/controls/window.h"
#include "../../gui/gui.h"
#include "../../library/console/console.h"
#include "../../library/input/input.h"
#include "../../library/security/xorstring.h"

using namespace gui;

#ifdef _DEBUG
static void print_control_tree(const std::string &prefix, const std::shared_ptr<control> &obj, bool is_last) {
    LOG("{}{}{} @ {:#x}", prefix, is_last ? "\xc0\xc4" : "\xc3\xc4",  //
        obj->get_object_name(), reinterpret_cast<uintptr_t>(obj.get()));

    const auto &children = obj->get_children();
    for (size_t i = 0; i < children.size(); i++) {
        print_control_tree(prefix + (!is_last ? "\xb3  " : "   "), children[i], i == children.size() - 1);
    }
}
#endif

static bool test_var = false;

void menu::initialize() {
    // An example on how the menu will be constructed

    // Step 1: Create the main window
    const auto main_window = std::make_shared<window>(xs("DEADCELL"));
    main_window->set_position({250, 250});
    main_window->set_size({850, 680});

    // Step 2: Add children to the main window or other controls
    main_window->add_child<title_bar>();

    const auto main_container = main_window->add_child<container>();
    main_container->set_flex_direction(FLEX_DIRECTION_COLUMN);
    main_container->set_justify_content(JUSTIFY_FLEX_START);
    main_container->set_spacing(6.0f);
    main_container->set_padding(16.0f, 16.0f);
    main_container->set_size(-1.0f);

    const auto master_switch_box = main_container->add_child<group_box>();
    master_switch_box->set_size({-1.0f, auto_value});

    const auto master_switch_container = master_switch_box->add_child<container>();
    master_switch_container->set_size({-1.0f, auto_value});
    master_switch_container->set_flex_direction(FLEX_DIRECTION_ROW);
    master_switch_container->set_justify_content(JUSTIFY_SPACE_BETWEEN);

    const auto master_checkbox = master_switch_container->add_child<checkbox>("Master switch", &test_var);
    master_checkbox->set_sub_text("Toggle all visual features on/off");
    master_checkbox->set_margin_br(0.0f);
    master_checkbox->set_align_self(ALIGN_CENTER);

    const auto master_button = master_switch_container->add_child<button>("Button", [](const auto &) {
        LOG("Button was pressed :3");
    });

    const auto bottom_container = main_container->add_child<container>();
    bottom_container->set_flex_direction(FLEX_DIRECTION_ROW);
    bottom_container->set_justify_content(JUSTIFY_FLEX_START);
    bottom_container->set_spacing(6.0f);
    bottom_container->set_size(-1.0f);

    const auto left_box = bottom_container->add_child<group_box>("Group box");
    left_box->set_size(-1.0f);

    const auto draw_layout_checkbox = left_box->add_child<checkbox>("Draw layout", &GET_BOOL("ui.draw_layout_bounds"));
    draw_layout_checkbox->set_sub_text("Visualize the UI layout nodes");

    left_box->add_child<checkbox>("Enabled", &test_var)->set_sub_text("Example checkbox sub-text");

    const auto right_container = bottom_container->add_child<container>();
    right_container->set_flex_direction(FLEX_DIRECTION_COLUMN);
    right_container->set_justify_content(JUSTIFY_FLEX_START);
    right_container->set_spacing(6.0f);
    right_container->set_size(-1.0f);

    const auto right_panel_top = right_container->add_child<group_box>("Group box");
    right_panel_top->set_size(-1.0f);
    right_panel_top->add_child<label>("Label");
    right_panel_top->add_child<button>("Example popup", [](const auto &) {
        const auto popup = std::make_shared<window>(xs("Popup window"));
        popup->set_position(input::get_mouse_pos());
        popup->set_size({250.0f, 300.0f});

        gui::add_popup_window(popup);
    });
    right_panel_top->add_child<button>("Example modal", [main_window](const auto &) {
        const auto modal = std::make_shared<window>(xs("Modal window"));
        modal->set_size({240.0f, 130.0f});
        modal->set_position(main_window->get_position() + main_window->get_size() * 0.5f - modal->get_size() * 0.5f);

        const auto main_container = modal->add_child<container>();
        main_container->set_flex_direction(FLEX_DIRECTION_COLUMN);
        main_container->set_align_items(ALIGN_CENTER);
        main_container->set_margin(auto_value, auto_value);
        main_container->set_size(auto_value);
        main_container->set_spacing(8.0f);

        const auto modal_label = main_container->add_child<label>("Example modal window");
        modal_label->set_margin_br(0.0f);

        const auto close_button = main_container->add_child<button>("Close", [modal](const auto &) {
            gui::remove_window(modal);
        });
        close_button->set_margin(0.0f, 0.0f);

        gui::add_modal_window(main_window, modal);
    });

    const auto right_panel_bottom = right_container->add_child<group_box>("Group box");
    right_panel_bottom->set_size(-1.0f);
    right_panel_bottom->add_child<label>("Label");
    right_panel_bottom->add_child<button>("Button", [](const auto &) {
        LOG("Button was pressed :3");
    });

    const auto test_text_input = right_panel_bottom->add_child<text_input>(&GET_STRING("test.string"));
    test_text_input->set_size({180.0f, 30.0f});

    // Step 3: Add window to the window manager
    gui::add_window(main_window);

    // Step 4: Create another window
    const auto another_window = std::make_shared<window>(xs("Another title"));
    another_window->set_position(16.0f);
    another_window->set_size({300.0f, 32.0f});
    another_window->add_child<watermark>("DEADCELL", "lol");

    // Step 5: Add the other window to the window manager
    gui::add_window(another_window);

#ifdef _DEBUG
    print_control_tree("", main_window, false);
    print_control_tree("", another_window, true);
#endif

    LOG("[+] Menu initialized");
}
