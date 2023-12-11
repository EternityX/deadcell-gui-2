#pragma once

#include "control.h"

namespace gui {

    class label : public control {
    private:
        std::string m_text;

    public:
        explicit label(std::string_view text);

        void layout(layout_node &parent) override;
        void render() override;

#ifdef _DEBUG
        std::string_view get_object_name() override {
            return "window";
        }
#endif

        std::string_view get_text() const {
            return m_text;
        }

        void set_text(std::string_view value) {
            m_text = value;
        }
    };

}  // namespace gui
