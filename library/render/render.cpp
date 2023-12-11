#define NANOSVG_IMPLEMENTATION
#include <nanosvg.h>

#define NANOSVGRAST_IMPLEMENTATION
#include <nanosvgrast.h>

#include <d3d11.h>
#include <span>
#include <unordered_map>

#include "../../core/patterns/patterns.h"
#include "../../library/console/console.h"
#include "../../library/input/input.h"
#include "../../source-sdk/classes/CEconItemDefinition.h"
#include "../../vendor/fonts/font_fa_defines.h"
#include "../../vendor/fonts/font_fa_solid_ttf.h"
#include "../../vendor/fonts/font_inter_regular_ttf.h"
#include "../../vendor/fonts/font_inter_semibold_ttf.h"
#include "../../vendor/fonts/font_pixelmix_ttf.h"
#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_freetype.h"
#include "../../vendor/imgui/imgui_impl_dx11.h"
#include "../../vendor/imgui/imgui_impl_win32.h"
#include "render.h"
#include "svg_parser.h"

#define DRAW_LIST ImGui::GetBackgroundDrawList()
#define TO_IMVEC2(...) ImVec2(std::floor((__VA_ARGS__).x), std::floor((__VA_ARGS__).y))
#define TO_IMCOL32(color) IM_COL32((color).r, (color).g, (color).b, (color).a)

struct RasterizedSvg {
    ID3D11ShaderResourceView *m_resource_view = nullptr;
    point_t m_size;
    float m_scale;
};

static ID3D11Device *d3d11_device;
static ID3D11DeviceContext *d3d11_context;
static ID3D11RenderTargetView *d3d11_render_target;

static std::unordered_map<draw_font_t, ImFont *> fonts;
static std::unordered_map<draw_svg_t, RasterizedSvg> svgs;

static ImFont *create_font_from_data(
    ImGuiIO &io,
    std::span<const uint8_t> font_data,
    float size,
    int flags = 0,
    const ImWchar *glyph_ranges = nullptr
) {
    ImFontConfig font_config;
    font_config.FontData = const_cast<uint8_t *>(font_data.data());
    font_config.FontDataSize = font_data.size();
    font_config.FontDataOwnedByAtlas = false;
    font_config.SizePixels = size;
    font_config.GlyphRanges = glyph_ranges;

    return io.Fonts->AddFont(&font_config);
}

static const ImWchar fa_regular_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};

static void create_font(draw_font_t id, font_family_t font_family, int font_size, uint32_t flags, bool apply_dpi_scaling) {
    auto &io = ImGui::GetIO();

    ImFont *font_ptr;
    switch (font_family) {
        case FONT_DEBUG: {
            ImFontConfig font_config;
            font_config.SizePixels = static_cast<float>(font_size);
            font_ptr = io.Fonts->AddFontDefault(&font_config);
            break;
        }
        case FONT_INTER_REGULAR: font_ptr = create_font_from_data(io, font_inter_regular_ttf, font_size, flags); break;
        case FONT_INTER_SEMIBOLD: font_ptr = create_font_from_data(io, font_inter_semibold_ttf, font_size, flags); break;
        case FONT_PIXELMIX: font_ptr = create_font_from_data(io, font_pixelmix_ttf, font_size, flags); break;
        case FONT_FA_SOLID: font_ptr = create_font_from_data(io, font_fa_solid_ttf, font_size, flags, fa_regular_ranges); break;
    }

    fonts.insert_or_assign(id, font_ptr);
}

static draw_svg_t create_svg(const char *path, float scale) {
    const auto svg_data = CSvgParser::parse(path);
    if (svg_data.empty()) {
        return 0;
    }

    const auto data_copy = strdup(svg_data.data());
    const auto image = nsvgParse(data_copy, xs("px"), 96.0f);  // 96 dpi is the standard for the svg format
    if (!image) {
        return 0;
    }

    free(data_copy);

    RasterizedSvg result;
    result.m_scale = scale;
    result.m_size = {static_cast<float>(image->width), static_cast<float>(image->height)};

    const auto rasterizer = nsvgCreateRasterizer();
    if (!rasterizer) {
        nsvgDelete(image);
        return 0;
    }

    const auto w = static_cast<int>(image->width * scale);
    const auto h = static_cast<int>(image->height * scale);
    const auto bytes = std::make_unique<uint8_t[]>(w * h * 4);

    nsvgRasterize(rasterizer, image, 0, 0, scale, bytes.get(), w, h, w * 4);
    nsvgDelete(image);
    nsvgDeleteRasterizer(rasterizer);

    D3D11_TEXTURE2D_DESC texture_desc = {};
    texture_desc.Width = w;
    texture_desc.Height = h;
    texture_desc.MipLevels = 1;
    texture_desc.ArraySize = 1;
    texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture_desc.SampleDesc.Count = 1;
    texture_desc.Usage = D3D11_USAGE_DEFAULT;
    texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texture_desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA resource_data = {};
    resource_data.pSysMem = bytes.get();
    resource_data.SysMemPitch = texture_desc.Width * 4;
    resource_data.SysMemSlicePitch = 0;

    ID3D11Texture2D *texture = nullptr;
    if (FAILED(d3d11_device->CreateTexture2D(&texture_desc, &resource_data, &texture))) {
        return 0;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC resource_view_desc = {};
    resource_view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    resource_view_desc.Texture2D.MipLevels = texture_desc.MipLevels;
    resource_view_desc.Texture2D.MostDetailedMip = 0;

    if (FAILED(d3d11_device->CreateShaderResourceView(texture, &resource_view_desc, &result.m_resource_view))) {
        texture->Release();
        return 0;
    }

    texture->Release();

    const auto hash = hash::fnv64(path);
    svgs.insert_or_assign(hash, result);
    return hash;
}

static void initialize_fonts(ImGuiIO &io) {
    render::debug_font = render::get_font(FONT_DEBUG, 13, FONT_FLAG_NONE);
    render::esp_name_font = render::get_font(FONT_INTER_REGULAR, 16, FONT_FLAG_FORCE_AUTO_HINT);
    render::esp_flags_font = render::get_font(FONT_PIXELMIX, 10, FONT_FLAG_MONO_HINTING | FONT_FLAG_MONOCHROME);

    render::menu_font_large_semibold = render::get_font(FONT_INTER_SEMIBOLD, 20, FONT_FLAG_FORCE_AUTO_HINT, true);
    render::menu_font_semibold = render::get_font(FONT_INTER_SEMIBOLD, 16, FONT_FLAG_FORCE_AUTO_HINT, true);
    render::menu_font_regular = render::get_font(FONT_INTER_SEMIBOLD, 16, FONT_FLAG_FORCE_AUTO_HINT, true);
    render::menu_font_small_semibold = render::get_font(FONT_INTER_SEMIBOLD, 14, FONT_FLAG_FORCE_AUTO_HINT, true);
    render::menu_font_small = render::get_font(FONT_INTER_REGULAR, 14, FONT_FLAG_FORCE_AUTO_HINT, true);
    render::menu_font_icon = render::get_font(FONT_FA_SOLID, 12, FONT_FLAG_MONO_HINTING | FONT_FLAG_MONOCHROME, true);
}

static void initialize_svgs() {
    render::svg_dots = create_svg(xs("panorama/images/backgrounds/dots.vsvg_c"), 1.0f);
    render::svg_knife_karambit = create_svg(xs("panorama/images/icons/equipment/knife_karambit.vsvg_c"), 1.0f);
    render::svg_knife_flip = create_svg(xs("panorama/images/icons/equipment/knife_flip.vsvg_c"), 1.0f);

    render::svg_decoy = create_svg(xs("panorama/images/icons/equipment/decoy.vsvg_c"), 0.5f);
    render::svg_hegrenade = create_svg(xs("panorama/images/icons/equipment/hegrenade.vsvg_c"), 0.5f);
    render::svg_molotov = create_svg(xs("panorama/images/icons/equipment/molotov.vsvg_c"), 0.5f);
    render::svg_incgrenade = create_svg(xs("panorama/images/icons/equipment/incgrenade.vsvg_c"), 0.5f);
    render::svg_flashbang = create_svg(xs("panorama/images/icons/equipment/flashbang.vsvg_c"), 0.5f);
    render::svg_smoke_grenade = create_svg(xs("panorama/images/icons/equipment/smokegrenade.vsvg_c"), 0.5f);

    auto it = reinterpret_cast<CEconItemDefinition **>(patterns::item_definition_list);

    for (size_t i = 0; i < 0x3f; i++, it += 4) {
        const auto item_def = *it;
        if (!item_def) {
            continue;
        }

        render::get_weapon_svg(item_def->weapon_name);
    }
}

static void initialize_imgui(ImGuiIO &io) {
    io.IniFilename = nullptr;

    initialize_fonts(io);
    initialize_svgs();
}

draw_svg_t render::get_weapon_svg(std::string_view weapon_name) {
    char path_buffer[256] = {};
    std::vformat_to(path_buffer, xs("panorama/images/icons/equipment/{}.vsvg_c"), std::make_format_args(weapon_name.substr(7)));

    const auto hash = hash::fnv64(path_buffer);
    if (const auto it = svgs.find(hash); it != svgs.end()) {
        return hash;
    }

    return create_svg(path_buffer, 4.5f);
}

void render::initialize(IDXGISwapChain *swap_chain) {
    DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
    swap_chain->GetDesc(&swap_chain_desc);

    ImGui::CreateContext();

    swap_chain->GetDevice(IID_PPV_ARGS(&d3d11_device));
    d3d11_device->GetImmediateContext(&d3d11_context);

    ImGui_ImplWin32_Init(swap_chain_desc.OutputWindow);
    ImGui_ImplDX11_Init(d3d11_device, d3d11_context);

    initialize_imgui(ImGui::GetIO());

    input::initialize(swap_chain_desc.OutputWindow);
}

void render::create_device_objects(IDXGISwapChain *swap_chain) {
    ID3D11Texture2D *back_buffer = nullptr;
    swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));

    if (back_buffer) {
        DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
        swap_chain->GetDesc(&swap_chain_desc);

        D3D11_RENDER_TARGET_VIEW_DESC desc = {};
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

        switch (swap_chain_desc.BufferDesc.Format) {
            case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB: desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
            default: desc.Format = swap_chain_desc.BufferDesc.Format; break;
        }

        if (FAILED(d3d11_device->CreateRenderTargetView(back_buffer, &desc, &d3d11_render_target))) {
            desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

            if (FAILED(d3d11_device->CreateRenderTargetView(back_buffer, &desc, &d3d11_render_target))) {
                d3d11_device->CreateRenderTargetView(back_buffer, nullptr, &d3d11_render_target);
            }
        }

        back_buffer->Release();
    }

    ImGui_ImplDX11_CreateDeviceObjects();
}

void render::destroy_device_objects() {
    ImGui_ImplDX11_InvalidateDeviceObjects();

    if (d3d11_render_target) {
        d3d11_render_target->Release();
        d3d11_render_target = nullptr;
    }
}

void render::begin_frame(IDXGISwapChain *swap_chain) {
    if (!d3d11_render_target) {
        create_device_objects(swap_chain);
    }

    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX11_NewFrame();

    ImGui::NewFrame();
}

void render::end_frame() {
    d3d11_context->OMSetRenderTargets(1, &d3d11_render_target, nullptr);

    ImGui::EndFrame();
    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void render::pop_clip() {
    DRAW_LIST->PopClipRect();
}

void render::push_clip_fullscreen() {
    DRAW_LIST->PushClipRectFullScreen();
}

void render::push_clip(point_t min, point_t max, bool intersect) {
    DRAW_LIST->PushClipRect(TO_IMVEC2(min), TO_IMVEC2(max), intersect);
}

draw_font_t render::get_font(font_family_t font_family, int font_size, uint32_t flags, bool apply_dpi_scaling) {
    const auto id = static_cast<uint32_t>(font_family) << 28 |  //
                    flags << 16 |                               //
                    static_cast<uint32_t>(font_size) << 10 |    //
                    static_cast<uint32_t>(apply_dpi_scaling);

    if (!fonts.contains(id)) {
        create_font(id, font_family, font_size, flags, apply_dpi_scaling);
    }

    return id;
}

void render::draw_line(point_t start, point_t end, color_t color, float thickness) {
    DRAW_LIST->AddLine(TO_IMVEC2(start), TO_IMVEC2(end), TO_IMCOL32(color), thickness);
}

void render::draw_rect(point_t pos, point_t size, color_t color, float rounding, draw_flags_t flags, float thickness) {
    DRAW_LIST->AddRect(TO_IMVEC2(pos), TO_IMVEC2(pos + size), TO_IMCOL32(color), rounding, flags, thickness);
}

void render::draw_triangle(point_t point1, point_t point2, point_t point3, color_t color, float thickness) {
    DRAW_LIST->AddTriangle(TO_IMVEC2(point1), TO_IMVEC2(point2), TO_IMVEC2(point3), TO_IMCOL32(color), thickness);
}

void render::draw_circle(point_t anchor, float radius, color_t color, float thickness) {
    DRAW_LIST->AddCircle(TO_IMVEC2(anchor), radius, TO_IMCOL32(color), 0, thickness);
}

void render::draw_svg(point_t pos, draw_svg_t svg, color_t color, float scale, float rounding, draw_flags_t flags) {
    const auto &[resource_view, size, _] = svgs[svg];

    DRAW_LIST->AddImageRounded(
        resource_view, TO_IMVEC2(pos), TO_IMVEC2(pos + size * scale), {0.0f, 0.0f}, {1.0f, 1.0f},  //
        TO_IMCOL32(color), rounding, flags
    );
}

void render::draw_progress_circle(
    point_t pos,
    float radius,
    draw_svg_t svg,
    color_t fill_color,
    color_t bg_color,
    float progress,
    float duration
) {
    constexpr float min = math::deg_to_rad(270.0f);

    const float max = math::deg_to_rad(270.0f + 1.0f + 360.0f * (progress / duration));
    if (min > max) {
        return;
    }

    // slight background tint for the icon
    fill_circle(pos, radius, {5, 5, 5, 155});

    // fill in background of the circle
    path_arc_to(pos, radius + 1.0f, min, max);
    path_stroke({bg_color.r, bg_color.b, bg_color.a, 65}, 0, 1.0f);

    // fill in the background of already depleted progress
    path_arc_to(pos, radius, math::deg_to_rad(0.0f), math::deg_to_rad(360.0f));
    path_stroke({bg_color.r, bg_color.b, bg_color.a, 185}, 0, 2.0f);

    // fill in actual progress
    path_arc_to(pos, radius - 2.0f, min, max);
    path_stroke(fill_color, 0, 2.0f);
    auto svg_size = render::get_svg_size(svg, 0.5f);
    draw_svg({pos.x - svg_size.x * 0.5f + 1.0f, pos.y - svg_size.x * 0.5f - 4.0f}, svg, {255, 255, 255}, 0.5f);
}

void render::draw_text(point_t pos, std::string_view text, draw_font_t font, color_t color, float wrap_width, int font_size) {
    const auto font_ptr = fonts[font];
    if (font_size < 0) {
        font_size = font_ptr->FontSize;
    }

    DRAW_LIST->AddText(
        font_ptr, font_size, TO_IMVEC2(pos), TO_IMCOL32(color),  //
        text.data(), text.data() + text.size(), wrap_width
    );
}

void render::draw_text_outlined(
    point_t pos,
    std::string_view text,
    draw_font_t font,
    color_t color,
    color_t color_outline,
    float wrap_width,
    int font_size
) {
    const auto font_ptr = fonts[font];
    if (font_size < 0) {
        font_size = font_ptr->FontSize;
    }

    // begin outline
    DRAW_LIST->AddText(
        font_ptr, font_size, TO_IMVEC2(pos + point_t(0.0f, 1.0f)), TO_IMCOL32(color_outline),  //
        text.data(), text.data() + text.size(), wrap_width
    );

    DRAW_LIST->AddText(
        font_ptr, font_size, TO_IMVEC2(pos + point_t(0.0f, -1.0f)), TO_IMCOL32(color_outline),  //
        text.data(), text.data() + text.size(), wrap_width
    );

    DRAW_LIST->AddText(
        font_ptr, font_size, TO_IMVEC2(pos + point_t(1.0f, 0.0f)), TO_IMCOL32(color_outline),  //
        text.data(), text.data() + text.size(), wrap_width
    );

    DRAW_LIST->AddText(
        font_ptr, font_size, TO_IMVEC2(pos + point_t(-1.0f, 0.0f)), TO_IMCOL32(color_outline),  //
        text.data(), text.data() + text.size(), wrap_width
    );
    // end outline

    DRAW_LIST->AddText(
        font_ptr, font_size, TO_IMVEC2(pos), TO_IMCOL32(color),  //
        text.data(), text.data() + text.size(), wrap_width
    );
}

void render::draw_text_drop_shadow(
    point_t pos,
    std::string_view text,
    draw_font_t font,
    color_t color,
    color_t color_shadow,
    float wrap_width,
    int font_size
) {
    const auto font_ptr = fonts[font];
    if (font_size < 0) {
        font_size = font_ptr->FontSize;
    }

    DRAW_LIST->AddText(
        font_ptr, font_size, TO_IMVEC2(pos + 1.0f), TO_IMCOL32(color_shadow),  //
        text.data(), text.data() + text.size(), wrap_width
    );

    DRAW_LIST->AddText(
        font_ptr, font_size, TO_IMVEC2(pos), TO_IMCOL32(color),  //
        text.data(), text.data() + text.size(), wrap_width
    );
}

void render::draw_shadow_rect(
    point_t pos,
    point_t size,
    color_t color,
    float thickness,
    float rounding,
    draw_flags_t flags,
    point_t offset
) {
    DRAW_LIST->AddShadowRect(
        TO_IMVEC2(pos), TO_IMVEC2(pos + size), TO_IMCOL32(color), thickness, TO_IMVEC2(offset), flags, rounding
    );
}

void render::draw_shadow_circle(point_t anchor, float radius, color_t color, float thickness, point_t offset) {
    DRAW_LIST->AddShadowCircle(TO_IMVEC2(anchor), radius, TO_IMCOL32(color), thickness, TO_IMVEC2(offset));
}

void render::fill_rect(point_t pos, point_t size, color_t color, float rounding, draw_flags_t flags) {
    DRAW_LIST->AddRectFilled(TO_IMVEC2(pos), TO_IMVEC2(pos + size), TO_IMCOL32(color), rounding, flags);
}

void render::fill_gradient(point_t pos, point_t size, color_t color1, color_t color2, bool horizontal) {
    DRAW_LIST->AddRectFilledMultiColor(
        TO_IMVEC2(pos), TO_IMVEC2(pos + size),                                     //
        TO_IMCOL32(color1), horizontal ? TO_IMCOL32(color2) : TO_IMCOL32(color1),  //
        TO_IMCOL32(color2), horizontal ? TO_IMCOL32(color1) : TO_IMCOL32(color2)
    );
}

void render::fill_triangle(point_t point1, point_t point2, point_t point3, color_t color) {
    DRAW_LIST->AddTriangleFilled(TO_IMVEC2(point1), TO_IMVEC2(point2), TO_IMVEC2(point3), TO_IMCOL32(color));
}

void render::fill_circle(point_t anchor, float radius, color_t color) {
    DRAW_LIST->AddCircleFilled(TO_IMVEC2(anchor), radius, TO_IMCOL32(color));
}

void render::path_arc_to(point_t pos, float radius, float min, float max, int segments) {
    DRAW_LIST->PathArcTo(TO_IMVEC2(pos), radius, min, max, segments);
}

void render::path_stroke(color_t color, int flags, float thickness) {
    DRAW_LIST->PathStroke(TO_IMCOL32(color), flags, thickness);
}

point_t render::get_display_size() {
    auto display_size = ImGui::GetIO().DisplaySize;
    return {display_size.x, display_size.y};
}

point_t render::get_text_size(std::string_view text, draw_font_t font, float wrap_width, int font_size) {
    const auto font_ptr = fonts[font];
    if (font_size < 0) {
        font_size = font_ptr->FontSize;
    }

    const auto size = font_ptr->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text.data(), text.data() + text.size());
    return {size.x, size.y};
}

point_t render::get_svg_size(draw_svg_t svg, float scale) {
    const auto &[resource_view, size, _] = svgs[svg];

    return size * scale;
}

float render::get_time() {
    return static_cast<float>(ImGui::GetTime());
}

float render::get_delta_time() {
    return ImGui::GetIO().DeltaTime;
}
