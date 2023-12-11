#pragma once

#include <memory>

#include "controls/window.h"

namespace gui {

    void add_window(const std::shared_ptr<window> &window);
    void add_modal_window(const std::shared_ptr<window> &parent_window, const std::shared_ptr<window> &window);
    void add_popup_window(const std::shared_ptr<window> &window);
    void remove_window(const std::shared_ptr<window> &window);

    void frame();

}  // namespace gui
