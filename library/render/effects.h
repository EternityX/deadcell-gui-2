#pragma once

#include "render.h"

namespace render::effects {

    void begin_frame(IDXGISwapChain *swap_chain);
    void create_device_objects(IDXGISwapChain *swap_chain);
    void destroy_device_objects();

    void draw_to_screen(point_t pos, point_t size, color_t color, float rounding = 0.0f, draw_flags_t flags = CORNER_ALL);
    void draw_to_screen_uv(
        point_t pos,
        point_t size,
        point_t uv_min,
        point_t uv_max,
        color_t color,
        float rounding = 0.0f,
        draw_flags_t flags = CORNER_ALL
    );

    void apply_blur(float size, float quality = 10.0f, float directions = 32.0f);

}  // namespace render::effects
