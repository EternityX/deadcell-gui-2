#pragma once

#include <cstdint>

class color_t {
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a = 255;

    static color_t blend(const color_t lhs, const color_t rhs, const float t) {
        return color_t(
            lhs.r + static_cast<int>(t * (rhs.r - lhs.r)), lhs.g + static_cast<int>(t * (rhs.g - lhs.g)),
            lhs.b + static_cast<int>(t * (rhs.b - lhs.b)), lhs.a + static_cast<int>(t * (rhs.a - lhs.a))
        );
    }
};