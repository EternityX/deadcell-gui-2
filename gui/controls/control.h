#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../library/math/point.h"
#include "../colors.h"
#include "../event.h"
#include "../layout.h"

namespace gui {

    constexpr float auto_value = std::numeric_limits<float>::quiet_NaN();

    class control : public std::enable_shared_from_this<control> {
    private:
        std::shared_ptr<control> m_parent;
        std::vector<std::shared_ptr<control>> m_children;

        bool m_visible = true;
        bool m_props_dirty = true;

        point_t m_size;
        point_t m_min_size = auto_value;
        point_t m_max_size = auto_value;
        point_t m_padding_tl = 0.0f;
        point_t m_padding_br = 0.0f;
        point_t m_margin_tl = 0.0f;
        point_t m_margin_br = 0.0f;

        float m_flex_grow = 0.0f;
        float m_flex_shrink = 1.0f;

        align_t m_align_self = ALIGN_AUTO;

    protected:
        layout_node m_node = nullptr;

        void apply_properties();

    public:
        control() = default;

        virtual ~control();

        // Returns whether the event was handled or not.
        virtual bool event(const base_event &ev) {
            return false;
        }

        virtual void layout(layout_node &parent) {
            if (!m_node) {
                m_node = parent.new_child();
            }

            if (m_node.get_parent() != parent) {
                m_node.remove_from_parent();
                parent.push_back(m_node);
            }

            apply_properties();
        }

        virtual void render() {
            for (const auto &child : m_children) {
                if (child->m_visible) {
                    child->render();
                }
            }
        }

#ifdef _DEBUG
        virtual std::string_view get_object_name() = 0;
#endif

        void add_child(const std::shared_ptr<control> &child);

        template <typename t, typename... args_t>
        std::shared_ptr<t> add_child(args_t &&...args) {
            const auto child = std::make_shared<t>(std::forward<args_t>(args)...);
            add_child(child);
            return child;
        }

        void remove_child(const std::shared_ptr<control> &control);
        void remove_all_children();
        void remove_from_parent();

        bool is_child(const std::shared_ptr<control> &control) const;
        bool is_ancestor(const std::shared_ptr<control> &control) const;

        template <typename t>
        bool is_a() const {
            return dynamic_cast<const t *>(this);
        }

        const std::shared_ptr<control> &get_parent() const {
            return m_parent;
        }

        const std::vector<std::shared_ptr<control>> &get_children() const {
            return m_children;
        }

        layout_node &get_node() {
            return m_node;
        }

        const layout_node &get_node() const {
            return m_node;
        }

        // Property getters

        bool get_visible() const {
            return m_visible;
        }

        point_t get_size() const {
            return m_size;
        }

        point_t get_min_size() const {
            return m_min_size;
        }

        point_t get_max_size() const {
            return m_max_size;
        }

        point_t get_padding_tl() const {
            return m_padding_tl;
        }

        point_t get_padding_br() const {
            return m_padding_br;
        }

        point_t get_margin_tl() const {
            return m_margin_tl;
        }

        point_t get_margin_br() const {
            return m_margin_br;
        }

        float get_flex_grow() const {
            return m_flex_grow;
        }

        float get_flex_shrink() const {
            return m_flex_shrink;
        }

        align_t get_align_self() const {
            return m_align_self;
        }

        // Property setters

        void set_visible(bool value) {
            m_visible = value;
        }

        void set_size(point_t value) {
            m_props_dirty = true;
            m_size = value;
        }

        void set_min_size(point_t value) {
            m_props_dirty = true;
            m_min_size = value;
        }

        void set_max_size(point_t value) {
            m_props_dirty = true;
            m_max_size = value;
        }

        void set_padding(point_t tl, point_t br) {
            set_padding_tl(tl);
            set_padding_br(br);
        }

        void set_padding_tl(point_t value) {
            m_props_dirty = true;
            m_padding_tl = value;
        }

        void set_padding_br(point_t value) {
            m_props_dirty = true;
            m_padding_br = value;
        }

        void set_margin(point_t tl, point_t br) {
            set_margin_tl(tl);
            set_margin_br(br);
        }

        void set_margin_tl(point_t value) {
            m_props_dirty = true;
            m_margin_tl = value;
        }

        void set_margin_br(point_t value) {
            m_props_dirty = true;
            m_margin_br = value;
        }

        void set_flex_grow(float value) {
            m_props_dirty = true;
            m_flex_grow = value;
        }

        void set_flex_shrink(float value) {
            m_props_dirty = true;
            m_flex_shrink = value;
        }

        void set_align_self(align_t value) {
            m_props_dirty = true;
            m_align_self = value;
        }
    };

}  // namespace gui
