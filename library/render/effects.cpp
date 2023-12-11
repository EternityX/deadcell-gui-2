#include <d3dcompiler.h>

#include "../../vendor/imgui/imgui.h"
#include "../console/console.h"
#include "effects.h"
#include "render.h"

#define DRAW_LIST ImGui::GetBackgroundDrawList()
#define TO_IMVEC2(...) ImVec2(std::floor((__VA_ARGS__).x), std::floor((__VA_ARGS__).y))
#define TO_IMCOL32(color) IM_COL32((color).r, (color).g, (color).b, (color).a)

struct [[gnu::aligned(16)]] blur_input_buffer_t {
    point_t m_texture_size;
    float m_blur_directions;
    float m_blur_quality;
    float m_blur_radius;
};

static ID3D11Device *d3d11_device;
static ID3D11DeviceContext *d3d11_context;
static ID3D11RenderTargetView *d3d11_render_target;
static ID3D11Texture2D *back_buffer_texture;
static ID3D11Texture2D *effect_texture;
static ID3D11SamplerState *effect_texture_sampler;
static ID3D11ShaderResourceView *back_buffer_resource_view;
static ID3D11ShaderResourceView *effect_resource_view;
static ID3D11PixelShader *blur_pixel_shader;
static ID3D11Buffer *blur_shader_cbuffer;
static ID3D11RenderTargetView *old_render_target_view;

// Blur
static blur_input_buffer_t blur_params;

static void blur_begin(const ImDrawList *draw_list, const ImDrawCmd *draw_cmd) {
    d3d11_context->OMGetRenderTargets(1, &old_render_target_view, nullptr);
    d3d11_context->OMSetRenderTargets(1, &d3d11_render_target, nullptr);

    D3D11_MAPPED_SUBRESOURCE blur_shader_cbuffer_subres;
    d3d11_context->Map(blur_shader_cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &blur_shader_cbuffer_subres);

    const auto blur_input = reinterpret_cast<blur_input_buffer_t *>(blur_shader_cbuffer_subres.pData);
    blur_input->m_texture_size = render::get_display_size();
    blur_input->m_blur_radius = blur_params.m_blur_radius;
    blur_input->m_blur_quality = blur_params.m_blur_quality;
    blur_input->m_blur_directions = blur_params.m_blur_directions;

    d3d11_context->Unmap(blur_shader_cbuffer, 0);
    d3d11_context->PSSetConstantBuffers(0, 1, &blur_shader_cbuffer);
    d3d11_context->PSSetSamplers(0, 1, &effect_texture_sampler);
    d3d11_context->PSSetShader(blur_pixel_shader, nullptr, 0);
}

static void blur_end(const ImDrawList *draw_list, const ImDrawCmd *draw_cmd) {
    d3d11_context->OMSetRenderTargets(1, &old_render_target_view, nullptr);
}

void render::effects::begin_frame(IDXGISwapChain *swap_chain) {
    if (!effect_resource_view) {
        effects::create_device_objects(swap_chain);
    }

    d3d11_context->CopyResource(effect_texture, back_buffer_texture);
}

void render::effects::create_device_objects(IDXGISwapChain *swap_chain) {
    swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer_texture));
    swap_chain->GetDevice(IID_PPV_ARGS(&d3d11_device));

    d3d11_device->GetImmediateContext(&d3d11_context);

    D3D11_TEXTURE2D_DESC texture_desc = {};
    back_buffer_texture->GetDesc(&texture_desc);
    d3d11_device->CreateTexture2D(&texture_desc, nullptr, &effect_texture);
    d3d11_device->CreateShaderResourceView(effect_texture, nullptr, &effect_resource_view);
    d3d11_device->CreateShaderResourceView(back_buffer_texture, nullptr, &back_buffer_resource_view);
    d3d11_device->CreateRenderTargetView(effect_texture, nullptr, &d3d11_render_target);

    D3D11_SAMPLER_DESC sampler_desc = {};
    sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    sampler_desc.MipLODBias = 0.0f;
    sampler_desc.MinLOD = 0.0f;
    sampler_desc.MaxLOD = 1.0f;

    d3d11_device->CreateSamplerState(&sampler_desc, &effect_texture_sampler);

    // @todo: Compile this ahead of time and embed a binary blob
    constexpr static const char blur_pixel_shader_source[] = R"(
        struct PS_INPUT
        {
            float4 pos   : SV_POSITION;
            float4 color : COLOR0;
            float2 uv    : TEXCOORD0;
        };

        cbuffer BlurInputBuffer : register(b0)
        {
            float2 texture_size;
            float blur_directions;
            float blur_quality;
            float blur_radius;
        };

        Texture2D back_texture : register(t0);
        sampler texture_sampler : register(s0);

        float4 main(PS_INPUT input) : SV_Target
        {
            const float PI_2 = 6.28318530718f;

            float2 radius = blur_radius / texture_size;
            float4 color = back_texture.Sample(texture_sampler, input.uv);

            float increment = 1.0f / blur_quality;
            for (float d = 0.0f; d < PI_2; d += PI_2 / blur_directions)
            {
                for (float i = increment; i < 1.00001f; i += increment)
                {
                    float2 blur_uv = input.uv + float2(cos(d), sin(d)) * radius * i;
                    color += back_texture.Sample(texture_sampler, blur_uv);
                }
            }

            return color / (blur_quality * blur_directions + 1.0f);
        }
    )";

    ID3DBlob *blur_pixel_shader_blob;

    if (D3DCompile(
            blur_pixel_shader_source, sizeof(blur_pixel_shader_source), nullptr, nullptr, nullptr, "main", "ps_4_0", 0, 0,
            &blur_pixel_shader_blob, nullptr
        ) != S_OK)  //
    {
        LOG("Failed to create blur pixel shader");
        return;
    }

    d3d11_device->CreatePixelShader(
        blur_pixel_shader_blob->GetBufferPointer(), blur_pixel_shader_blob->GetBufferSize(), nullptr, &blur_pixel_shader
    );

    blur_pixel_shader_blob->Release();

    D3D11_BUFFER_DESC blur_cbuffer_desc = {};

    blur_cbuffer_desc.ByteWidth = sizeof(blur_input_buffer_t);
    blur_cbuffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    blur_cbuffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    blur_cbuffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    d3d11_device->CreateBuffer(&blur_cbuffer_desc, nullptr, &blur_shader_cbuffer);
}

void render::effects::destroy_device_objects() {
    // @todo: Do this lol
}

void render::effects::draw_to_screen(point_t pos, point_t size, color_t color, float rounding, draw_flags_t flags) {
    draw_to_screen_uv(pos, size, 0.0f, 1.0f, color, rounding, flags);
}

void render::effects::draw_to_screen_uv(
    point_t pos,
    point_t size,
    point_t uv_min,
    point_t uv_max,
    color_t color,
    float rounding,
    draw_flags_t flags
) {
    DRAW_LIST->AddImageRounded(
        reinterpret_cast<ImTextureID>(effect_resource_view), TO_IMVEC2(pos), TO_IMVEC2(pos + size),  //
        {uv_min.x, uv_min.y}, {uv_max.x, uv_max.y}, TO_IMCOL32(color), rounding, flags
    );
}

void render::effects::apply_blur(float size, float quality, float directions) {
    blur_params.m_blur_radius = size;
    blur_params.m_blur_quality = quality;
    blur_params.m_blur_directions = directions;

    DRAW_LIST->AddCallback(blur_begin, nullptr);
    DRAW_LIST->AddImage(
        reinterpret_cast<ImTextureID>(back_buffer_resource_view),  //
        {0.0f, 0.0f}, TO_IMVEC2(render::get_display_size()), {0.0f, 0.0f}, {1.0f, 1.0f}, IM_COL32(255, 255, 255, 255)
    );
    DRAW_LIST->AddCallback(blur_end, nullptr);
    DRAW_LIST->AddCallback(ImDrawCallback_ResetRenderState, nullptr);
}
