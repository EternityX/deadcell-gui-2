#pragma once

#include <functional>

#include "../../library/math/animation.h"
#include "control.h"

namespace gui {

    class button : public control {
    private:
        std::string m_text;
        std::string m_icon;

        std::function<void(const std::shared_ptr<control> &)> m_callback_func;

        bool m_enabled;
        bool m_hovered = false;
        bool m_clicked = false;

        layout_node m_inner_node = nullptr;
        layout_node m_icon_node = nullptr;
        layout_node m_text_node = nullptr;

        math::animation<float> m_hovered_anim;

    public:
        button(std::string_view text, const std::function<void(const std::shared_ptr<control> &)> &callback = {});
        button(
            std::string_view text,
            std::string_view icon,
            const std::function<void(const std::shared_ptr<control> &)> &callback = {}
        );

        bool event(const base_event &ev) override;
        void layout(layout_node &parent) override;
        void render() override;

#ifdef _DEBUG
        std::string_view get_object_name() override {
            return "button";
        }
#endif

        std::string_view get_text() const {
            return m_text;
        }

        std::string_view get_icon_text() const {
            return m_icon;
        }

        bool get_enabled() const {
            return m_enabled;
        }

        void set_text(std::string_view value) {
            m_text = value;
        }

        void set_icon_text(std::string_view value) {
            m_icon = value;

            if (m_icon.empty()) {
                m_icon_node = nullptr;
            }
        }

        void set_enabled(bool enabled) {
            m_enabled = enabled;
        }
    };

}  // namespace gui