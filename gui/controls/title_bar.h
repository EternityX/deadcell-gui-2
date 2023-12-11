#pragma once

#include "control.h"
#include "window.h"

namespace gui {

    class title_bar : public control {
    private:
        std::shared_ptr<window> get_parent_window() const;

        layout_node m_text_node = nullptr;

    public:
        explicit title_bar();

        bool event(const base_event &ev) override;
        void layout(layout_node &parent) override;
        void render() override;

#ifdef _DEBUG
        std::string_view get_object_name() override {
            return "title_bar";
        }
#endif
    };

}  // namespace gui
