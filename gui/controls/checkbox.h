#pragma once

#include <functional>

#include "../../library/math/animation.h"
#include "control.h"

namespace gui {

    class checkbox : public control {
    private:
        std::string m_text;
        std::string m_sub_text;

        std::function<void(const std::shared_ptr<control> &)> m_callback_func;

        bool *m_checked;
        bool m_enabled;
        bool m_hovered = false;
        bool m_clicked = false;

        layout_node m_check_node = nullptr;
        layout_node m_label_node = nullptr;
        layout_node m_text_node = nullptr;
        layout_node m_sub_text_node = nullptr;

        math::animation<float> m_hovered_anim;
        math::animation<float> m_toggled_anim;

    public:
        checkbox(std::string_view text, bool *var, const std::function<void(const std::shared_ptr<control> &)> &callback = {});

        bool event(const base_event &ev) override;
        void layout(layout_node &parent) override;
        void render() override;

#ifdef _DEBUG
        std::string_view get_object_name() override {
            return "checkbox";
        }
#endif

        std::string_view get_text() const {
            return m_text;
        }

        std::string_view get_sub_text() const {
            return m_sub_text;
        }

        bool get_enabled() const {
            return m_enabled;
        }

        void set_text(std::string_view value) {
            m_text = value;
        }

        void set_sub_text(std::string_view value) {
            m_sub_text = value;

            if (m_sub_text.empty()) {
                m_sub_text_node = nullptr;
            }
        }

        void set_enabled(bool enabled) {
            m_enabled = enabled;
        }
    };

}  // namespace gui