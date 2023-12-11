#pragma once

#include <d3d11.h>
#include <dxgi.h>

#include <cstdint>
#include <string_view>

#include "../math/math.h"
#include "color.h"

enum draw_flags_t {
    CORNER_TL = 1 << 4,
    CORNER_TR = 1 << 5,
    CORNER_BL = 1 << 6,
    CORNER_BR = 1 << 7,
    CORNER_NONE = 1 << 8,
    CORNER_TOP = CORNER_TL | CORNER_TR,
    CORNER_BOTTOM = CORNER_BL | CORNER_BR,
    CORNER_LEFT = CORNER_TL | CORNER_BL,
    CORNER_RIGHT = CORNER_TR | CORNER_BR,
    CORNER_ALL = CORNER_TL | CORNER_TR | CORNER_BL | CORNER_BR,
};

enum font_flags_t : uint16_t {
    FONT_FLAG_NONE = 0,
    FONT_FLAG_NO_HINT = 1 << 0,
    FONT_FLAG_NO_AUTO_HINT = 1 << 1,
    FONT_FLAG_FORCE_AUTO_HINT = 1 << 2,
    FONT_FLAG_LIGHT_HINTING = 1 << 3,
    FONT_FLAG_MONO_HINTING = 1 << 4,
    FONT_FLAG_BOLD = 1 << 5,
    FONT_FLAG_SLANTED = 1 << 6,
    FONT_FLAG_MONOCHROME = 1 << 7,
    FONT_FLAG_LOAD_COLOR = 1 << 8,
    FONT_FLAG_BITMAP = 1 << 9,
};

using draw_font_t = uint32_t;
using draw_svg_t = uint64_t;

enum font_family_t {
    FONT_DEBUG,
    FONT_INTER_REGULAR,
    FONT_INTER_SEMIBOLD,
    FONT_PIXELMIX,
    FONT_FA_SOLID,
};

namespace render {

    inline draw_font_t debug_font;
    inline draw_font_t esp_name_font;
    inline draw_font_t esp_flags_font;

    inline draw_font_t menu_large_font;
    inline draw_font_t menu_font_large_semibold;
    inline draw_font_t menu_font_semibold;
    inline draw_font_t menu_italic_font;
    inline draw_font_t menu_font_regular;
    inline draw_font_t menu_font_small_semibold;
    inline draw_font_t menu_font_small;
    inline draw_font_t menu_font_icon;

    inline draw_svg_t svg_dots;
    inline draw_svg_t svg_knife_karambit;
    inline draw_svg_t svg_knife_flip;
    inline draw_svg_t svg_decoy;
    inline draw_svg_t svg_hegrenade;
    inline draw_svg_t svg_smoke_grenade;
    inline draw_svg_t svg_molotov;
    inline draw_svg_t svg_incgrenade;
    inline draw_svg_t svg_flashbang;

    draw_svg_t get_weapon_svg(std::string_view weapon_name);

    void initialize(IDXGISwapChain *swap_chain);
    void create_device_objects(IDXGISwapChain *swap_chain);
    void destroy_device_objects();

    void begin_frame(IDXGISwapChain *swap_chain);
    void end_frame();

    void pop_clip();
    void push_clip_fullscreen();
    void push_clip(point_t min, point_t max, bool intersect = true);

    draw_font_t get_font(font_family_t font_family, int font_size, uint32_t flags, bool apply_dpi_scaling = false);

    // clang-format off
    void draw_line(point_t start, point_t end, color_t color, float thickness = 1.0f);
    void draw_rect(point_t pos, point_t size, color_t color, float rounding = 0.0f, draw_flags_t flags = CORNER_ALL, float thickness = 1.0f);
    void draw_triangle(point_t point1, point_t point2, point_t point3, color_t color, float thickness = 1.0f);
    void draw_circle(point_t anchor, float radius, color_t color, float thickness = 1.0f);
    void draw_svg(point_t pos, draw_svg_t svg, color_t color = {255, 255, 255}, float scale = 1.0f, float rounding = 0.0f, draw_flags_t flags = CORNER_ALL);
    void draw_progress_circle(point_t pos, float radius, draw_svg_t svg, color_t fill_color, color_t bg_color, float progress = 1.0f, float duration = 1.0f);

    void draw_text(point_t pos, std::string_view text, draw_font_t font, color_t color, float wrap_width = 0.0f, int font_size = -1);
    void draw_text_outlined(point_t pos, std::string_view text, draw_font_t font, color_t color, color_t color_outline = {6, 6, 6, 220}, float wrap_width = 0.0f, int font_size = -1);
    void draw_text_drop_shadow(point_t pos, std::string_view text, draw_font_t font, color_t color, color_t color_shadow = {6, 6, 6, 220}, float wrap_width = 0.0f, int font_size = -1);

    void draw_shadow_rect(point_t pos, point_t size, color_t color, float thickness, float rounding = 0.0f, draw_flags_t flags = CORNER_ALL, point_t offset = {0.0f, 0.0f});
    void draw_shadow_circle(point_t anchor, float radius, color_t color, float thickness, point_t offset = {0.0f, 0.0f});

    void fill_rect(point_t pos, point_t size, color_t color, float rounding = 0.0f, draw_flags_t flags = CORNER_ALL);
    void fill_gradient(point_t pos, point_t size, color_t color1, color_t color2, bool horizontal = true);
    void fill_triangle(point_t point1, point_t point2, point_t point3, color_t color);
    void fill_circle(point_t anchor, float radius, color_t color);

    void path_arc_to(point_t pos, float radius, float min, float max, int segments = 0);
    void path_stroke(color_t color, int flags = 0, float thickness = 1.0f);
    // clang-format on

    point_t get_display_size();
    point_t get_text_size(std::string_view text, draw_font_t font, float wrap_width = 0.0f, int font_size = -1);
    point_t get_svg_size(draw_svg_t svg, float scale = 1.0f);

    float get_time();
    float get_delta_time();

}  // namespace render