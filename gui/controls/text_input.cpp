#include "text_input.h"
#include "../../core/settings/settings.h"
#include "../../library/console/console.h"
#include "../../library/input/input.h"
#include "../../library/render/render.h"

const char *ImTextCharToUtf8(char out_buf[5], unsigned int c);
int ImTextCharFromUtf8(unsigned int *out_char, const char *in_text, const char *in_text_end);

#define STB_TEXTEDIT_IMPLEMENTATION
#define STB_TEXTEDIT_STRING gui::text_input::state_t

static size_t get_codepoint_byte_offset(std::string_view string, size_t codepoint_idx) {
    size_t offset = 0;
    for (size_t i = 0; i < codepoint_idx; i++) {
        unsigned int wc;
        offset += ImTextCharFromUtf8(&wc, string.data() + offset, string.data() + string.size());
    }

    return offset;
}

static uint32_t get_codepoint_at_index(std::string_view string, size_t codepoint_idx) {
    unsigned int wc;
    for (size_t i = 0, offset = 0; i < codepoint_idx; i++) {
        offset += ImTextCharFromUtf8(&wc, string.data() + offset, string.data() + string.size());
    }

    return wc;
}

struct measurements_t {
    point_t m_offset;
    point_t m_size;
    int m_num_chars;
};

static measurements_t measure_range(  //
    std::string_view string,
    draw_font_t font,
    int font_size,
    size_t start_idx,
    size_t end_idx,
    bool end_at_newline
) {
    const auto start_offset = get_codepoint_byte_offset(string, start_idx);
    const auto end_offset = get_codepoint_byte_offset(string, end_idx);
    const auto line_size = render::get_char_advance('A', font, font_size);

    measurements_t result = {
        .m_offset = {0.0f, line_size.y},
        .m_size = {0.0f, line_size.y},
    };

    size_t byte_offset = 0;
    while (byte_offset < start_offset) {
        unsigned int wc;
        byte_offset += ImTextCharFromUtf8(&wc, string.data() + byte_offset, string.data() + string.size());
        const auto char_advance = render::get_char_advance(wc, font, font_size);
        result.m_offset.x += char_advance.x;
    }

    while (byte_offset < end_offset) {
        if (string[byte_offset] == '\n' && end_at_newline) {
            break;
        }

        unsigned int wc;
        byte_offset += ImTextCharFromUtf8(&wc, string.data() + byte_offset, string.data() + string.size());
        const auto char_advance = render::get_char_advance(wc, font, font_size);
        result.m_size.x += char_advance.x;
        result.m_num_chars++;
    }

    return result;
}

static int STB_TEXTEDIT_STRINGLEN(const STB_TEXTEDIT_STRING *obj) {
    const auto string = obj->m_value;

    size_t length = 0;
    for (size_t offset = 0; offset < string->size(); length++) {
        unsigned int wc;
        offset += ImTextCharFromUtf8(&wc, string->data() + offset, string->data() + string->size());
    }

    return length;
}

static void STB_TEXTEDIT_LAYOUTROW(StbTexteditRow *r, STB_TEXTEDIT_STRING *obj, int line_start_idx) {
    const auto string = obj->m_value;
    const auto string_len = STB_TEXTEDIT_STRINGLEN(obj);
    const auto measured = measure_range(*string, obj->m_font, obj->m_font_size, line_start_idx, string_len, true);

    r->x0 = 0.0f;
    r->x1 = measured.m_size.x;
    r->baseline_y_delta = measured.m_size.y;
    r->ymin = 0.0f;
    r->ymax = measured.m_size.y;
    r->num_chars = measured.m_num_chars;
}

static float STB_TEXTEDIT_GETWIDTH(STB_TEXTEDIT_STRING *obj, int line_start_idx, int char_idx) {
    const auto string = obj->m_value;
    const auto wc = get_codepoint_at_index(*string, line_start_idx + char_idx);
    const auto char_advance = render::get_char_advance(wc, render::menu_font_small);

    return char_advance.x;
}

static int STB_TEXTEDIT_KEYTOTEXT(int key) {
    return key & 0xFFF;
}

static STB_TEXTEDIT_CHARTYPE STB_TEXTEDIT_GETCHAR(const STB_TEXTEDIT_STRING *obj, int idx) {
    return get_codepoint_at_index(*obj->m_value, idx);
}

static void STB_TEXTEDIT_DELETECHARS(STB_TEXTEDIT_STRING *obj, int pos, int n) {
    const auto string = obj->m_value;
    const auto offset = get_codepoint_byte_offset(*string, pos);

    for (size_t i = 0; i < n; i++) {
        unsigned int wc;
        const auto char_len = ImTextCharFromUtf8(&wc, string->data() + offset, string->data() + string->size());
        string->erase(offset, char_len);
    }
}

static bool STB_TEXTEDIT_INSERTCHARS(STB_TEXTEDIT_STRING *obj, int pos, const STB_TEXTEDIT_CHARTYPE *new_text, int new_text_len) {
    const auto string = obj->m_value;
    const auto string_len = STB_TEXTEDIT_STRINGLEN(obj);

    auto offset = get_codepoint_byte_offset(*string, pos);
    for (size_t i = 0; i < new_text_len && string_len + i < obj->m_max_length; i++) {
        char mb_buffer[5];
        const auto mb_char = ImTextCharToUtf8(mb_buffer, new_text[i]);
        const auto char_len = strlen(mb_char);
        string->insert(string->begin() + offset, mb_buffer, mb_buffer + char_len);
        offset += char_len;
    }

    return true;
}

#define STB_TEXTEDIT_K_CTRL 0x1000
#define STB_TEXTEDIT_K_SHIFT 0x2000
#define STB_TEXTEDIT_K_ALT 0x4000
#define STB_TEXTEDIT_K_LEFT KEY_LEFT_ARROW
#define STB_TEXTEDIT_K_RIGHT KEY_RIGHT_ARROW
#define STB_TEXTEDIT_K_UP KEY_UP_ARROW
#define STB_TEXTEDIT_K_DOWN KEY_DOWN_ARROW
#define STB_TEXTEDIT_K_PGUP KEY_PAGE_UP
#define STB_TEXTEDIT_K_PGDOWN KEY_PAGE_DOWN
#define STB_TEXTEDIT_K_LINESTART KEY_HOME
#define STB_TEXTEDIT_K_LINEEND KEY_END
#define STB_TEXTEDIT_K_TEXTSTART (STB_TEXTEDIT_K_CTRL | STB_TEXTEDIT_K_LINESTART)
#define STB_TEXTEDIT_K_TEXTEND (STB_TEXTEDIT_K_CTRL | STB_TEXTEDIT_K_LINEEND)
#define STB_TEXTEDIT_K_DELETE KEY_DELETE
#define STB_TEXTEDIT_K_BACKSPACE KEY_BACKSPACE
#define STB_TEXTEDIT_K_UNDO (STB_TEXTEDIT_K_CTRL | KEY_Z)
#define STB_TEXTEDIT_K_REDO (STB_TEXTEDIT_K_CTRL | KEY_Y)

#include <stb_textedit.h>

gui::text_input::text_input(std::string *value) : text_input("", value) {}

gui::text_input::text_input(std::string_view placeholder, std::string *value) {
    set_placeholder(placeholder);
    set_max_length(-1);
    set_masked(false);
    set_enabled(true);
    set_margin_br({0.0f, 10.0f});
    set_size({auto_value, 30.0f});
    set_font(render::menu_font_regular);
    set_font_size(-1);

    m_last_input = render::get_time();
    m_state.m_value = value;

    stb_textedit_clear_state(&m_stb_state, true);
}

bool gui::text_input::event(const base_event &ev) {
    if (ev.type() == mouse_event::mouse_click && m_enabled) {
        const auto [pos, size] = m_node.get_rect();
        const auto hovered = input::is_mouse_in_bounds(pos, pos + size);

        if (hovered && !m_dragging) {
            const auto mouse_ev = static_cast<const mouse_event &>(ev);
            const auto text_size = render::get_text_size(*m_state.m_value, m_state.m_font, 0.0f, m_state.m_font_size);
            const auto text_pos = point_t(pos.x + 6.0f, pos.y + size.y * 0.5f - text_size.y * 0.5f);
            const auto click_pos = mouse_ev.position() - text_pos;

            stb_textedit_click(&m_state, &m_stb_state, click_pos.x, click_pos.y);
            m_dragging = true;
        }

        m_active = hovered;
        m_last_input = render::get_time();
        return true;
    } else if (ev.type() == mouse_event::mouse_move && m_dragging) {
        const auto [pos, size] = m_node.get_rect();
        const auto mouse_ev = static_cast<const mouse_event &>(ev);
        const auto text_size = render::get_text_size(*m_state.m_value, m_state.m_font, 0.0f, m_state.m_font_size);
        const auto text_pos = point_t(pos.x + 6.0f, pos.y + size.y * 0.5f - text_size.y * 0.5f);
        const auto click_pos = mouse_ev.position() - text_pos;

        stb_textedit_drag(&m_state, &m_stb_state, click_pos.x, click_pos.y);
        return true;
    } else if (ev.type() == mouse_event::mouse_up && m_dragging) {
        m_dragging = false;
        return true;
    } else if (ev.type() == window_event::focus_out && m_active) {
        m_active = false;
        return true;
    }

    if (!m_active) {
        return false;
    }

    if (ev.type() == key_event::text_char) {
        const auto &key_ev = static_cast<const key_event &>(ev);
        if (!iswprint(key_ev.key())) {
            return false;
        }

        m_last_input = render::get_time();
        stb_textedit_key(
            &m_state, &m_stb_state,
            key_ev.key() |                                                 //
                STB_TEXTEDIT_K_CTRL * static_cast<int>(key_ev.ctrl()) |    //
                STB_TEXTEDIT_K_SHIFT * static_cast<int>(key_ev.shift()) |  //
                STB_TEXTEDIT_K_ALT * static_cast<int>(key_ev.alt())
        );
        return true;
    } else if (ev.type() == key_event::key_down) {
        const auto &key_ev = static_cast<const key_event &>(ev);

        uint32_t stb_key_code = 0;
        switch (static_cast<key_code_t>(key_ev.key())) {
            case KEY_LEFT_ARROW: stb_key_code = STB_TEXTEDIT_K_LEFT; break;
            case KEY_RIGHT_ARROW: stb_key_code = STB_TEXTEDIT_K_RIGHT; break;
            case KEY_UP_ARROW: stb_key_code = STB_TEXTEDIT_K_UP; break;
            case KEY_DOWN_ARROW: stb_key_code = STB_TEXTEDIT_K_DOWN; break;
            case KEY_PAGE_UP: stb_key_code = STB_TEXTEDIT_K_PGUP; break;
            case KEY_PAGE_DOWN: stb_key_code = STB_TEXTEDIT_K_PGDOWN; break;
            case KEY_HOME: stb_key_code = STB_TEXTEDIT_K_LINESTART; break;
            case KEY_END: stb_key_code = STB_TEXTEDIT_K_LINEEND; break;
            case KEY_DELETE: stb_key_code = STB_TEXTEDIT_K_DELETE; break;
            case KEY_BACKSPACE: stb_key_code = STB_TEXTEDIT_K_BACKSPACE; break;
            case KEY_ENTER: stb_key_code = STB_TEXTEDIT_NEWLINE; break;
            default: break;
        }

        if (!stb_key_code) {
            return false;
        }

        m_last_input = render::get_time();
        stb_textedit_key(
            &m_state, &m_stb_state,
            stb_key_code |                                                 //
                STB_TEXTEDIT_K_CTRL * static_cast<int>(key_ev.ctrl()) |    //
                STB_TEXTEDIT_K_SHIFT * static_cast<int>(key_ev.shift()) |  //
                STB_TEXTEDIT_K_ALT * static_cast<int>(key_ev.alt())
        );
        return true;
    }

    return false;
}

void gui::text_input::layout(layout_node &parent) {
    control::layout(parent);
}

void gui::text_input::render() {
    const auto [pos, size] = m_node.get_rect();

    // base
    render::fill_rect(pos, size, colors::gray_700, 6.0f);

    // border
    render::draw_rect(pos, size, colors::accent_border, 6.0f);

    const auto string = m_state.m_value;
    const auto text_size = render::get_text_size(*string, m_state.m_font, 0.0f, m_state.m_font_size);
    const auto text_pos = point_t(pos.x + 6.0f, pos.y + size.y * 0.5f - text_size.y * 0.5f);

    if (m_active) {
        // select
        if (m_stb_state.select_start != m_stb_state.select_end) {
            const auto accent_color = GET_COLOR("accent_color");
            const auto measured = measure_range(               //
                *string, m_state.m_font, m_state.m_font_size,  //
                std::min(m_stb_state.select_start, m_stb_state.select_end),
                std::max(m_stb_state.select_start, m_stb_state.select_end), false
            );
            render::fill_rect(
                {text_pos.x + measured.m_offset.x, text_pos.y}, measured.m_size,  //
                {accent_color.r, accent_color.g, accent_color.b, 100}
            );
        }

        // cursor
        const auto measured = measure_range(
            *string, m_state.m_font, m_state.m_font_size,  //
            m_stb_state.cursor, m_stb_state.cursor, false
        );
        render::draw_line(
            {text_pos.x + measured.m_offset.x, text_pos.y},  //
            {text_pos.x + measured.m_offset.x, text_pos.y + measured.m_offset.y},
            colors::slate_400 / (std::fmod(render::get_time() - m_last_input, 1.0f) < 0.5f ? 50 : 0)
        );
    }

    // text
    render::draw_text(text_pos, *m_state.m_value, m_state.m_font, colors::slate_400, 0.0f, m_state.m_font_size);
}
