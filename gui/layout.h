#pragma once

#include "../library/math/point.h"

#include <Yoga.h>

namespace gui {

    struct auto_t {};

    constexpr auto_t _auto{};

    struct percent_t {
        float m_value;

        constexpr operator float() const {
            return m_value;
        }
    };

    constexpr percent_t pct(float value) {
        return {value};
    }

    constexpr percent_t operator""_pct(long double value) {
        return {static_cast<float>(value)};
    }

    enum align_t {
        ALIGN_AUTO = YGAlignAuto,
        ALIGN_FLEX_START = YGAlignFlexStart,
        ALIGN_CENTER = YGAlignCenter,
        ALIGN_FLEX_END = YGAlignFlexEnd,
        ALIGN_STRETCH = YGAlignStretch,
        ALIGN_BASELINE = YGAlignBaseline,
        ALIGN_SPACE_BETWEEN = YGAlignSpaceBetween,
        ALIGN_SPACE_AROUND = YGAlignSpaceAround,
    };

    enum dimension_t {
        DIMENSION_WIDTH = YGDimensionWidth,
        DIMENSION_HEIGHT = YGDimensionHeight,
    };

    enum direction_t {
        DIRECTION_INHERIT = YGDirectionInherit,
        DIRECTION_LTR = YGDirectionLTR,
        DIRECTION_RTL = YGDirectionRTL,
    };

    enum display_t {
        DISPLAY_FLEX = YGDisplayFlex,
        DISPLAY_NONE = YGDisplayNone,
    };

    enum edge_t {
        EDGE_LEFT = YGEdgeLeft,
        EDGE_TOP = YGEdgeTop,
        EDGE_RIGHT = YGEdgeRight,
        EDGE_BOTTOM = YGEdgeBottom,
        EDGE_START = YGEdgeStart,
        EDGE_END = YGEdgeEnd,
        EDGE_HORIZONTAL = YGEdgeHorizontal,
        EDGE_VERTICAL = YGEdgeVertical,
        EDGE_ALL = YGEdgeAll,
    };

    enum flex_direction_t {
        FLEX_DIRECTION_COLUMN = YGFlexDirectionColumn,
        FLEX_DIRECTION_COLUMN_REVERSE = YGFlexDirectionColumnReverse,
        FLEX_DIRECTION_ROW = YGFlexDirectionRow,
        FLEX_DIRECTION_ROW_REVERSE = YGFlexDirectionRowReverse,
    };

    enum gutter_t {
        GUTTER_COLUMN = YGGutterColumn,
        GUTTER_ROW = YGGutterRow,
        GUTTER_ALL = YGGutterAll,
    };

    enum justify_t {
        JUSTIFY_FLEX_START = YGJustifyFlexStart,
        JUSTIFY_CENTER = YGJustifyCenter,
        JUSTIFY_FLEX_END = YGJustifyFlexEnd,
        JUSTIFY_SPACE_BETWEEN = YGJustifySpaceBetween,
        JUSTIFY_SPACE_AROUND = YGJustifySpaceAround,
        JUSTIFY_SPACE_EVENLY = YGJustifySpaceEvenly,
    };

    enum measure_mode_t {
        MEASURE_MODE_UNDEFINED = YGMeasureModeUndefined,
        MEASURE_MODE_EXACTLY = YGMeasureModeExactly,
        MEASURE_MODE_AT_MOST = YGMeasureModeAtMost,
    };

    enum node_type_t {
        NODE_TYPE_DEFAULT = YGNodeTypeDefault,
        NODE_TYPE_TEXT = YGNodeTypeText,
    };

    enum overflow_t {
        OVERFLOW_VISIBLE = YGOverflowVisible,
        OVERFLOW_HIDDEN = YGOverflowHidden,
        OVERFLOW_SCROLL = YGOverflowScroll,
    };

    enum position_type_t {
        POSITION_TYPE_STATIC = YGPositionTypeStatic,
        POSITION_TYPE_RELATIVE = YGPositionTypeRelative,
        POSITION_TYPE_ABSOLUTE = YGPositionTypeAbsolute,
    };

    enum unit_t {
        UNIT_UNDEFINED = YGUnitUndefined,
        UNIT_POINT = YGUnitPoint,
        UNIT = YGUnitPercent,
        UNIT_AUTO = YGUnitAuto,
    };

    enum wrap_t {
        WRAP_NO_WRAP = YGWrapNoWrap,
        WRAP_WRAP = YGWrapWrap,
        WRAP_WRAP_REVERSE = YGWrapWrapReverse,
    };

    struct rect_t {
        point_t m_pos;
        point_t m_size;
    };

    class layout_node {
    public:
        layout_node(std::nullptr_t) : m_node(nullptr) {}

        layout_node() {
            static YGConfigRef config = nullptr;
            if (!config) {
                config = YGConfigNew();
                YGConfigSetUseWebDefaults(config, true);
            }

            m_node = YGNodeNewWithConfig(config);
        }

        layout_node(layout_node &&other) {
            m_node = other.m_node;
            other.m_node = nullptr;
        }

        layout_node &operator=(layout_node &&other) {
            m_node = other.m_node;
            other.m_node = nullptr;
            return *this;
        }

        ~layout_node() {
            if (!m_node || !m_owned) {
                return;
            }

            YGNodeFreeRecursive(m_node);
            m_node = nullptr;
        }

        point_t get_position() const {
            point_t result{};
            for (auto node = m_node; node; node = YGNodeGetParent(node)) {
                result.x += YGNodeLayoutGetLeft(node);
                result.y += YGNodeLayoutGetTop(node);
            }
            return result;
        }

        point_t get_size() const {
            return {YGNodeLayoutGetWidth(m_node), YGNodeLayoutGetHeight(m_node)};
        }

        rect_t get_rect() const {
            return {get_position(), get_size()};
        }

        void insert(const layout_node &node, size_t index) {
            YGNodeInsertChild(m_node, node.m_node, index);
        }

        void push_back(const layout_node &node) {
            insert(node, child_count());
        }

        void push_front(const layout_node &node) {
            insert(node, 0);
        }

        layout_node new_child() {
            layout_node result;
            push_back(result);
            return result;
        }

        size_t child_count() const {
            return YGNodeGetChildCount(m_node);
        }

        layout_node at(size_t index) const {
            return {YGNodeGetChild(m_node, index), false};
        }

        layout_node get_parent() const {
            return {YGNodeGetParent(m_node), false};
        }

        void remove_all_children() {
            YGNodeRemoveAllChildren(m_node);
        }

        void remove_from_parent() {
            if (const auto parent = get_parent()) {
                YGNodeRemoveChild(parent.m_node, m_node);
            }
        }

        void node_type(node_type_t type) {
            YGNodeSetNodeType(m_node, static_cast<YGNodeType>(type));
        }

        void calculate(float available_width, float available_height, direction_t direction) {
            YGNodeCalculateLayout(m_node, available_width, available_height, static_cast<YGDirection>(direction));
        }

        void direction(direction_t value) {
            YGNodeStyleSetDirection(m_node, static_cast<YGDirection>(value));
        }

        void flex_direction(flex_direction_t value) {
            YGNodeStyleSetFlexDirection(m_node, static_cast<YGFlexDirection>(value));
        }

        void justify_content(justify_t value) {
            YGNodeStyleSetJustifyContent(m_node, static_cast<YGJustify>(value));
        }

        void align_content(align_t value) {
            YGNodeStyleSetAlignContent(m_node, static_cast<YGAlign>(value));
        }

        void align_items(align_t value) {
            YGNodeStyleSetAlignItems(m_node, static_cast<YGAlign>(value));
        }

        void align_self(align_t value) {
            YGNodeStyleSetAlignSelf(m_node, static_cast<YGAlign>(value));
        }

        void position_type(position_type_t value) {
            YGNodeStyleSetPositionType(m_node, static_cast<YGPositionType>(value));
        }

        void flex_wrap(wrap_t value) {
            YGNodeStyleSetFlexWrap(m_node, static_cast<YGWrap>(value));
        }

        void overflow(overflow_t value) {
            YGNodeStyleSetOverflow(m_node, static_cast<YGOverflow>(value));
        }

        void display(display_t value) {
            YGNodeStyleSetDisplay(m_node, static_cast<YGDisplay>(value));
        }

        void flex(float value) {
            YGNodeStyleSetFlex(m_node, value);
        }

        void flex_grow(float value) {
            YGNodeStyleSetFlexGrow(m_node, value);
        }

        void flex_shrink(float value) {
            YGNodeStyleSetFlexShrink(m_node, value);
        }

        void flex_basis(float value) {
            YGNodeStyleSetFlexBasis(m_node, value);
        }

        void flex_basis(percent_t value) {
            YGNodeStyleSetFlexBasisPercent(m_node, value);
        }

        void flex_basis(auto_t) {
            YGNodeStyleSetFlexBasisAuto(m_node);
        }

        void position(edge_t edge, float value) {
            YGNodeStyleSetPosition(m_node, static_cast<YGEdge>(edge), value);
        }

        void position(edge_t edge, percent_t value) {
            YGNodeStyleSetPositionPercent(m_node, static_cast<YGEdge>(edge), value);
        }

        void margin(edge_t edge, float value) {
            YGNodeStyleSetMargin(m_node, static_cast<YGEdge>(edge), value);
        }

        void margin(edge_t edge, percent_t value) {
            YGNodeStyleSetMarginPercent(m_node, static_cast<YGEdge>(edge), value);
        }

        void margin(edge_t edge, auto_t) {
            YGNodeStyleSetMarginAuto(m_node, static_cast<YGEdge>(edge));
        }

        void padding(edge_t edge, float value) {
            YGNodeStyleSetPadding(m_node, static_cast<YGEdge>(edge), value);
        }

        void padding(edge_t edge, percent_t value) {
            YGNodeStyleSetPaddingPercent(m_node, static_cast<YGEdge>(edge), value);
        }

        void border(edge_t edge, float value) {
            YGNodeStyleSetBorder(m_node, static_cast<YGEdge>(edge), value);
        }

        void gap(gutter_t gutter, float value) {
            YGNodeStyleSetGap(m_node, static_cast<YGGutter>(gutter), value);
        }

        void width(float value) {
            YGNodeStyleSetWidth(m_node, value);
        }

        void width(percent_t value) {
            YGNodeStyleSetWidthPercent(m_node, value);
        }

        void width(auto_t) {
            YGNodeStyleSetWidthAuto(m_node);
        }

        void height(float value) {
            YGNodeStyleSetHeight(m_node, value);
        }

        void height(percent_t value) {
            YGNodeStyleSetHeightPercent(m_node, value);
        }

        void height(auto_t) {
            YGNodeStyleSetHeightAuto(m_node);
        }

        void min_width(float value) {
            YGNodeStyleSetMinWidth(m_node, value);
        }

        void min_width(percent_t value) {
            YGNodeStyleSetMinWidthPercent(m_node, value);
        }

        void min_height(float value) {
            YGNodeStyleSetMinHeight(m_node, value);
        }

        void min_height(percent_t value) {
            YGNodeStyleSetMinHeightPercent(m_node, value);
        }

        void max_width(float value) {
            YGNodeStyleSetMaxWidth(m_node, value);
        }

        void max_width(percent_t value) {
            YGNodeStyleSetMaxWidthPercent(m_node, value);
        }

        void max_height(float value) {
            YGNodeStyleSetMaxHeight(m_node, value);
        }

        void max_height(percent_t value) {
            YGNodeStyleSetMaxHeightPercent(m_node, value);
        }

        void aspect_ratio(float value) {
            YGNodeStyleSetAspectRatio(m_node, value);
        }

        operator bool() const {
            return m_node;
        }

    private:
        layout_node(YGNodeRef node, bool owned) : m_node(node), m_owned(owned) {}

        YGNodeRef m_node;

        bool m_owned = true;
    };

}  // namespace gui
