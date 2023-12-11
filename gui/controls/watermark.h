#pragma once

#include "control.h"
#include "window.h"
#include <string>

namespace gui {

    class watermark : public control {
    private:
        std::shared_ptr<window> get_parent_window() const;

        layout_node m_text_node = nullptr;
        layout_node m_ver_text_node = nullptr;
        std::string m_text;
        std::string m_ver_text;

    public:
        explicit watermark(std::string_view text, std::string_view ver_text);

        bool event(const base_event &ev) override;
        void layout(layout_node &parent) override;
        void render() override;

#ifdef _DEBUG
        std::string_view get_object_name() override {
            return "watermark";
        }
#endif
    };

}  // namespace gui