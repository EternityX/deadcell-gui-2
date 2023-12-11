#pragma once

#include "control.h"

namespace gui {

    class group_box : public control {
    private:
        std::string m_text;

        layout_node m_inner_node = nullptr;
        layout_node m_header_node = nullptr;
        layout_node m_text_node = nullptr;

    public:
        explicit group_box();
        explicit group_box(std::string_view name);

        void layout(layout_node &parent) override;
        void render() override;

#ifdef _DEBUG
        std::string_view get_object_name() override {
            return "group_box";
        }
#endif

        std::string_view get_text() const {
            return m_text;
        }

        void set_text(std::string_view value) {
            m_text = value;

            if (m_text.empty()) {
                m_header_node = nullptr;
                m_text_node = nullptr;
            }
        }
    };

}  // namespace gui
