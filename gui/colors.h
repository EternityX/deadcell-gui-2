#pragma once

#include "../library/render/color.h"

namespace gui::colors {

    struct color_wrapper_t {
        uint8_t r, g, b;

        constexpr operator color_t() const {
            return {r, g, b};
        }

        constexpr color_t operator/(float opacity) const {
            return {r, g, b, static_cast<uint8_t>(255.0f * opacity)};
        }

        constexpr color_t operator/(int opacity) const {
            return {r, g, b, static_cast<uint8_t>(2.55f * static_cast<float>(opacity))};
        }
    };

    constexpr static color_wrapper_t slate_50 = {0xf8, 0xfa, 0xfc};
    constexpr static color_wrapper_t slate_100 = {0xf1, 0xf5, 0xf9};
    constexpr static color_wrapper_t slate_200 = {0xe2, 0xe8, 0xf0};
    constexpr static color_wrapper_t slate_300 = {0xcb, 0xd5, 0xe1};
    constexpr static color_wrapper_t slate_400 = {0x94, 0xa3, 0xb8};
    constexpr static color_wrapper_t slate_500 = {0x64, 0x74, 0x8b};
    constexpr static color_wrapper_t slate_600 = {0x47, 0x55, 0x69};
    constexpr static color_wrapper_t slate_700 = {0x33, 0x41, 0x55};
    constexpr static color_wrapper_t slate_800 = {0x1e, 0x29, 0x3b};
    constexpr static color_wrapper_t slate_900 = {0x0f, 0x17, 0x2a};
    constexpr static color_wrapper_t slate_950 = {0x02, 0x06, 0x17};

    constexpr static color_wrapper_t gray_50 = {0xf9, 0xfa, 0xfb};
    constexpr static color_wrapper_t gray_100 = {0xf3, 0xf4, 0xf6};
    constexpr static color_wrapper_t gray_200 = {0xe5, 0xe7, 0xeb};
    constexpr static color_wrapper_t gray_300 = {0xd1, 0xd5, 0xdb};
    constexpr static color_wrapper_t gray_400 = {0x9c, 0xa3, 0xaf};
    constexpr static color_wrapper_t gray_500 = {0x6b, 0x72, 0x80};
    constexpr static color_wrapper_t gray_600 = {0x4b, 0x55, 0x63};
    constexpr static color_wrapper_t gray_700 = {0x37, 0x41, 0x51};
    constexpr static color_wrapper_t gray_750 = {0x2B, 0x35, 0x44};
    constexpr static color_wrapper_t gray_800 = {0x1f, 0x29, 0x37};
    constexpr static color_wrapper_t gray_900 = {0x11, 0x18, 0x27};
    constexpr static color_wrapper_t gray_950 = {0x03, 0x07, 0x12};

    constexpr static color_wrapper_t zinc_50 = {0xfa, 0xfa, 0xfa};
    constexpr static color_wrapper_t zinc_100 = {0xf4, 0xf4, 0xf5};
    constexpr static color_wrapper_t zinc_200 = {0xe4, 0xe4, 0xe7};
    constexpr static color_wrapper_t zinc_300 = {0xd4, 0xd4, 0xd8};
    constexpr static color_wrapper_t zinc_400 = {0xa1, 0xa1, 0xaa};
    constexpr static color_wrapper_t zinc_500 = {0x71, 0x71, 0x7a};
    constexpr static color_wrapper_t zinc_600 = {0x52, 0x52, 0x5b};
    constexpr static color_wrapper_t zinc_700 = {0x3f, 0x3f, 0x46};
    constexpr static color_wrapper_t zinc_800 = {0x27, 0x27, 0x2a};
    constexpr static color_wrapper_t zinc_900 = {0x18, 0x18, 0x1b};
    constexpr static color_wrapper_t zinc_950 = {0x09, 0x09, 0x0b};

    constexpr static color_wrapper_t neutral_50 = {0xfa, 0xfa, 0xfa};
    constexpr static color_wrapper_t neutral_100 = {0xf5, 0xf5, 0xf5};
    constexpr static color_wrapper_t neutral_200 = {0xe5, 0xe5, 0xe5};
    constexpr static color_wrapper_t neutral_300 = {0xd4, 0xd4, 0xd4};
    constexpr static color_wrapper_t neutral_400 = {0xa3, 0xa3, 0xa3};
    constexpr static color_wrapper_t neutral_500 = {0x73, 0x73, 0x73};
    constexpr static color_wrapper_t neutral_600 = {0x52, 0x52, 0x52};
    constexpr static color_wrapper_t neutral_700 = {0x40, 0x40, 0x40};
    constexpr static color_wrapper_t neutral_800 = {0x26, 0x26, 0x26};
    constexpr static color_wrapper_t neutral_900 = {0x17, 0x17, 0x17};
    constexpr static color_wrapper_t neutral_950 = {0x0a, 0x0a, 0x0a};

    constexpr static color_wrapper_t stone_50 = {0xfa, 0xfa, 0xf9};
    constexpr static color_wrapper_t stone_100 = {0xf5, 0xf5, 0xf4};
    constexpr static color_wrapper_t stone_200 = {0xe7, 0xe5, 0xe4};
    constexpr static color_wrapper_t stone_300 = {0xd6, 0xd3, 0xd1};
    constexpr static color_wrapper_t stone_400 = {0xa8, 0xa2, 0x9e};
    constexpr static color_wrapper_t stone_500 = {0x78, 0x71, 0x6c};
    constexpr static color_wrapper_t stone_600 = {0x57, 0x53, 0x4e};
    constexpr static color_wrapper_t stone_700 = {0x44, 0x40, 0x3c};
    constexpr static color_wrapper_t stone_800 = {0x29, 0x25, 0x24};
    constexpr static color_wrapper_t stone_900 = {0x1c, 0x19, 0x17};
    constexpr static color_wrapper_t stone_950 = {0x0c, 0x0a, 0x09};

    constexpr static color_wrapper_t red_50 = {0xfe, 0xf2, 0xf2};
    constexpr static color_wrapper_t red_100 = {0xfe, 0xe2, 0xe2};
    constexpr static color_wrapper_t red_200 = {0xfe, 0xca, 0xca};
    constexpr static color_wrapper_t red_300 = {0xfc, 0xa5, 0xa5};
    constexpr static color_wrapper_t red_400 = {0xf8, 0x71, 0x71};
    constexpr static color_wrapper_t red_500 = {0xef, 0x44, 0x44};
    constexpr static color_wrapper_t red_600 = {0xdc, 0x26, 0x26};
    constexpr static color_wrapper_t red_700 = {0xb9, 0x1c, 0x1c};
    constexpr static color_wrapper_t red_800 = {0x99, 0x1b, 0x1b};
    constexpr static color_wrapper_t red_900 = {0x7f, 0x1d, 0x1d};
    constexpr static color_wrapper_t red_950 = {0x45, 0x0a, 0x0a};

    constexpr static color_wrapper_t orange_50 = {0xff, 0xf7, 0xed};
    constexpr static color_wrapper_t orange_100 = {0xff, 0xed, 0xd5};
    constexpr static color_wrapper_t orange_200 = {0xfe, 0xd7, 0xaa};
    constexpr static color_wrapper_t orange_300 = {0xfd, 0xba, 0x74};
    constexpr static color_wrapper_t orange_400 = {0xfb, 0x92, 0x3c};
    constexpr static color_wrapper_t orange_500 = {0xf9, 0x73, 0x16};
    constexpr static color_wrapper_t orange_600 = {0xea, 0x58, 0x0c};
    constexpr static color_wrapper_t orange_700 = {0xc2, 0x41, 0x0c};
    constexpr static color_wrapper_t orange_800 = {0x9a, 0x34, 0x12};
    constexpr static color_wrapper_t orange_900 = {0x7c, 0x2d, 0x12};
    constexpr static color_wrapper_t orange_950 = {0x43, 0x14, 0x07};

    constexpr static color_wrapper_t amber_50 = {0xff, 0xfb, 0xeb};
    constexpr static color_wrapper_t amber_100 = {0xfe, 0xf3, 0xc7};
    constexpr static color_wrapper_t amber_200 = {0xfd, 0xe6, 0x8a};
    constexpr static color_wrapper_t amber_300 = {0xfc, 0xd3, 0x4d};
    constexpr static color_wrapper_t amber_400 = {0xfb, 0xbf, 0x24};
    constexpr static color_wrapper_t amber_500 = {0xf5, 0x9e, 0x0b};
    constexpr static color_wrapper_t amber_600 = {0xd9, 0x77, 0x06};
    constexpr static color_wrapper_t amber_700 = {0xb4, 0x53, 0x09};
    constexpr static color_wrapper_t amber_800 = {0x92, 0x40, 0x0e};
    constexpr static color_wrapper_t amber_900 = {0x78, 0x35, 0x0f};
    constexpr static color_wrapper_t amber_950 = {0x45, 0x1a, 0x03};

    constexpr static color_wrapper_t yellow_50 = {0xfe, 0xfc, 0xe8};
    constexpr static color_wrapper_t yellow_100 = {0xfe, 0xf9, 0xc3};
    constexpr static color_wrapper_t yellow_200 = {0xfe, 0xf0, 0x8a};
    constexpr static color_wrapper_t yellow_300 = {0xfd, 0xe0, 0x47};
    constexpr static color_wrapper_t yellow_400 = {0xfa, 0xcc, 0x15};
    constexpr static color_wrapper_t yellow_500 = {0xea, 0xb3, 0x08};
    constexpr static color_wrapper_t yellow_600 = {0xca, 0x8a, 0x04};
    constexpr static color_wrapper_t yellow_700 = {0xa1, 0x62, 0x07};
    constexpr static color_wrapper_t yellow_800 = {0x85, 0x4d, 0x0e};
    constexpr static color_wrapper_t yellow_900 = {0x71, 0x3f, 0x12};
    constexpr static color_wrapper_t yellow_950 = {0x42, 0x20, 0x06};

    constexpr static color_wrapper_t lime_50 = {0xf7, 0xfe, 0xe7};
    constexpr static color_wrapper_t lime_100 = {0xec, 0xfc, 0xcb};
    constexpr static color_wrapper_t lime_200 = {0xd9, 0xf9, 0x9d};
    constexpr static color_wrapper_t lime_300 = {0xbe, 0xf2, 0x64};
    constexpr static color_wrapper_t lime_400 = {0xa3, 0xe6, 0x35};
    constexpr static color_wrapper_t lime_500 = {0x84, 0xcc, 0x16};
    constexpr static color_wrapper_t lime_600 = {0x65, 0xa3, 0x0d};
    constexpr static color_wrapper_t lime_700 = {0x4d, 0x7c, 0x0f};
    constexpr static color_wrapper_t lime_800 = {0x3f, 0x62, 0x12};
    constexpr static color_wrapper_t lime_900 = {0x36, 0x53, 0x14};
    constexpr static color_wrapper_t lime_950 = {0x1a, 0x2e, 0x05};

    constexpr static color_wrapper_t green_50 = {0xf0, 0xfd, 0xf4};
    constexpr static color_wrapper_t green_100 = {0xdc, 0xfc, 0xe7};
    constexpr static color_wrapper_t green_200 = {0xbb, 0xf7, 0xd0};
    constexpr static color_wrapper_t green_300 = {0x86, 0xef, 0xac};
    constexpr static color_wrapper_t green_400 = {0x4a, 0xde, 0x80};
    constexpr static color_wrapper_t green_500 = {0x22, 0xc5, 0x5e};
    constexpr static color_wrapper_t green_600 = {0x16, 0xa3, 0x4a};
    constexpr static color_wrapper_t green_700 = {0x15, 0x80, 0x3d};
    constexpr static color_wrapper_t green_800 = {0x16, 0x65, 0x34};
    constexpr static color_wrapper_t green_900 = {0x14, 0x53, 0x2d};
    constexpr static color_wrapper_t green_950 = {0x05, 0x2e, 0x16};

    constexpr static color_wrapper_t emerald_50 = {0xec, 0xfd, 0xf5};
    constexpr static color_wrapper_t emerald_100 = {0xd1, 0xfa, 0xe5};
    constexpr static color_wrapper_t emerald_200 = {0xa7, 0xf3, 0xd0};
    constexpr static color_wrapper_t emerald_300 = {0x6e, 0xe7, 0xb7};
    constexpr static color_wrapper_t emerald_400 = {0x34, 0xd3, 0x99};
    constexpr static color_wrapper_t emerald_500 = {0x10, 0xb9, 0x81};
    constexpr static color_wrapper_t emerald_600 = {0x05, 0x96, 0x69};
    constexpr static color_wrapper_t emerald_700 = {0x04, 0x78, 0x57};
    constexpr static color_wrapper_t emerald_800 = {0x06, 0x5f, 0x46};
    constexpr static color_wrapper_t emerald_900 = {0x06, 0x4e, 0x3b};
    constexpr static color_wrapper_t emerald_950 = {0x02, 0x2c, 0x22};

    constexpr static color_wrapper_t teal_50 = {0xf0, 0xfd, 0xfa};
    constexpr static color_wrapper_t teal_100 = {0xcc, 0xfb, 0xf1};
    constexpr static color_wrapper_t teal_200 = {0x99, 0xf6, 0xe4};
    constexpr static color_wrapper_t teal_300 = {0x5e, 0xea, 0xd4};
    constexpr static color_wrapper_t teal_400 = {0x2d, 0xd4, 0xbf};
    constexpr static color_wrapper_t teal_500 = {0x14, 0xb8, 0xa6};
    constexpr static color_wrapper_t teal_600 = {0x0d, 0x94, 0x88};
    constexpr static color_wrapper_t teal_700 = {0x0f, 0x76, 0x6e};
    constexpr static color_wrapper_t teal_800 = {0x11, 0x5e, 0x59};
    constexpr static color_wrapper_t teal_900 = {0x13, 0x4e, 0x4a};
    constexpr static color_wrapper_t teal_950 = {0x04, 0x2f, 0x2e};

    constexpr static color_wrapper_t cyan_50 = {0xec, 0xfe, 0xff};
    constexpr static color_wrapper_t cyan_100 = {0xcf, 0xfa, 0xfe};
    constexpr static color_wrapper_t cyan_200 = {0xa5, 0xf3, 0xfc};
    constexpr static color_wrapper_t cyan_300 = {0x67, 0xe8, 0xf9};
    constexpr static color_wrapper_t cyan_400 = {0x22, 0xd3, 0xee};
    constexpr static color_wrapper_t cyan_500 = {0x06, 0xb6, 0xd4};
    constexpr static color_wrapper_t cyan_600 = {0x08, 0x91, 0xb2};
    constexpr static color_wrapper_t cyan_700 = {0x0e, 0x74, 0x90};
    constexpr static color_wrapper_t cyan_800 = {0x15, 0x5e, 0x75};
    constexpr static color_wrapper_t cyan_900 = {0x16, 0x4e, 0x63};
    constexpr static color_wrapper_t cyan_950 = {0x08, 0x33, 0x44};

    constexpr static color_wrapper_t sky_50 = {0xf0, 0xf9, 0xff};
    constexpr static color_wrapper_t sky_100 = {0xe0, 0xf2, 0xfe};
    constexpr static color_wrapper_t sky_200 = {0xba, 0xe6, 0xfd};
    constexpr static color_wrapper_t sky_300 = {0x7d, 0xd3, 0xfc};
    constexpr static color_wrapper_t sky_400 = {0x38, 0xbd, 0xf8};
    constexpr static color_wrapper_t sky_500 = {0x0e, 0xa5, 0xe9};
    constexpr static color_wrapper_t sky_600 = {0x02, 0x84, 0xc7};
    constexpr static color_wrapper_t sky_700 = {0x03, 0x69, 0xa1};
    constexpr static color_wrapper_t sky_800 = {0x07, 0x59, 0x85};
    constexpr static color_wrapper_t sky_900 = {0x0c, 0x4a, 0x6e};
    constexpr static color_wrapper_t sky_950 = {0x08, 0x2f, 0x49};

    constexpr static color_wrapper_t blue_50 = {0xef, 0xf6, 0xff};
    constexpr static color_wrapper_t blue_100 = {0xdb, 0xea, 0xfe};
    constexpr static color_wrapper_t blue_200 = {0xbf, 0xdb, 0xfe};
    constexpr static color_wrapper_t blue_300 = {0x93, 0xc5, 0xfd};
    constexpr static color_wrapper_t blue_400 = {0x60, 0xa5, 0xfa};
    constexpr static color_wrapper_t blue_500 = {0x3b, 0x82, 0xf6};
    constexpr static color_wrapper_t blue_600 = {0x25, 0x63, 0xeb};
    constexpr static color_wrapper_t blue_700 = {0x1d, 0x4e, 0xd8};
    constexpr static color_wrapper_t blue_800 = {0x1e, 0x40, 0xaf};
    constexpr static color_wrapper_t blue_900 = {0x1e, 0x3a, 0x8a};
    constexpr static color_wrapper_t blue_950 = {0x17, 0x25, 0x54};

    constexpr static color_wrapper_t indigo_50 = {0xee, 0xf2, 0xff};
    constexpr static color_wrapper_t indigo_100 = {0xe0, 0xe7, 0xff};
    constexpr static color_wrapper_t indigo_200 = {0xc7, 0xd2, 0xfe};
    constexpr static color_wrapper_t indigo_300 = {0xa5, 0xb4, 0xfc};
    constexpr static color_wrapper_t indigo_400 = {0x81, 0x8c, 0xf8};
    constexpr static color_wrapper_t indigo_500 = {0x63, 0x66, 0xf1};
    constexpr static color_wrapper_t indigo_600 = {0x4f, 0x46, 0xe5};
    constexpr static color_wrapper_t indigo_700 = {0x43, 0x38, 0xca};
    constexpr static color_wrapper_t indigo_800 = {0x37, 0x30, 0xa3};
    constexpr static color_wrapper_t indigo_900 = {0x31, 0x2e, 0x81};
    constexpr static color_wrapper_t indigo_950 = {0x1e, 0x1b, 0x4b};

    constexpr static color_wrapper_t violet_50 = {0xf5, 0xf3, 0xff};
    constexpr static color_wrapper_t violet_100 = {0xed, 0xe9, 0xfe};
    constexpr static color_wrapper_t violet_200 = {0xdd, 0xd6, 0xfe};
    constexpr static color_wrapper_t violet_300 = {0xc4, 0xb5, 0xfd};
    constexpr static color_wrapper_t violet_400 = {0xa7, 0x8b, 0xfa};
    constexpr static color_wrapper_t violet_500 = {0x8b, 0x5c, 0xf6};
    constexpr static color_wrapper_t violet_600 = {0x7c, 0x3a, 0xed};
    constexpr static color_wrapper_t violet_700 = {0x6d, 0x28, 0xd9};
    constexpr static color_wrapper_t violet_800 = {0x5b, 0x21, 0xb6};
    constexpr static color_wrapper_t violet_900 = {0x4c, 0x1d, 0x95};
    constexpr static color_wrapper_t violet_950 = {0x2e, 0x10, 0x65};

    constexpr static color_wrapper_t purple_50 = {0xfa, 0xf5, 0xff};
    constexpr static color_wrapper_t purple_100 = {0xf3, 0xe8, 0xff};
    constexpr static color_wrapper_t purple_200 = {0xe9, 0xd5, 0xff};
    constexpr static color_wrapper_t purple_300 = {0xd8, 0xb4, 0xfe};
    constexpr static color_wrapper_t purple_400 = {0xc0, 0x84, 0xfc};
    constexpr static color_wrapper_t purple_500 = {0xa8, 0x55, 0xf7};
    constexpr static color_wrapper_t purple_600 = {0x93, 0x33, 0xea};
    constexpr static color_wrapper_t purple_700 = {0x7e, 0x22, 0xce};
    constexpr static color_wrapper_t purple_800 = {0x6b, 0x21, 0xa8};
    constexpr static color_wrapper_t purple_900 = {0x58, 0x1c, 0x87};
    constexpr static color_wrapper_t purple_950 = {0x3b, 0x07, 0x64};

    constexpr static color_wrapper_t fuchsia_50 = {0xfd, 0xf4, 0xff};
    constexpr static color_wrapper_t fuchsia_100 = {0xfa, 0xe8, 0xff};
    constexpr static color_wrapper_t fuchsia_200 = {0xf5, 0xd0, 0xfe};
    constexpr static color_wrapper_t fuchsia_300 = {0xf0, 0xab, 0xfc};
    constexpr static color_wrapper_t fuchsia_400 = {0xe8, 0x79, 0xf9};
    constexpr static color_wrapper_t fuchsia_500 = {0xd9, 0x46, 0xef};
    constexpr static color_wrapper_t fuchsia_600 = {0xc0, 0x26, 0xd3};
    constexpr static color_wrapper_t fuchsia_700 = {0xa2, 0x1c, 0xaf};
    constexpr static color_wrapper_t fuchsia_800 = {0x86, 0x19, 0x8f};
    constexpr static color_wrapper_t fuchsia_900 = {0x70, 0x1a, 0x75};
    constexpr static color_wrapper_t fuchsia_950 = {0x4a, 0x04, 0x4e};

    constexpr static color_wrapper_t pink_50 = {0xfd, 0xf2, 0xf8};
    constexpr static color_wrapper_t pink_100 = {0xfc, 0xe7, 0xf3};
    constexpr static color_wrapper_t pink_200 = {0xfb, 0xcf, 0xe8};
    constexpr static color_wrapper_t pink_300 = {0xf9, 0xa8, 0xd4};
    constexpr static color_wrapper_t pink_400 = {0xf4, 0x72, 0xb6};
    constexpr static color_wrapper_t pink_500 = {0xec, 0x48, 0x99};
    constexpr static color_wrapper_t pink_600 = {0xdb, 0x27, 0x77};
    constexpr static color_wrapper_t pink_700 = {0xbe, 0x18, 0x5d};
    constexpr static color_wrapper_t pink_800 = {0x9d, 0x17, 0x4d};
    constexpr static color_wrapper_t pink_900 = {0x83, 0x18, 0x43};
    constexpr static color_wrapper_t pink_950 = {0x50, 0x07, 0x24};

    constexpr static color_wrapper_t rose_50 = {0xff, 0xf1, 0xf2};
    constexpr static color_wrapper_t rose_100 = {0xff, 0xe4, 0xe6};
    constexpr static color_wrapper_t rose_200 = {0xfe, 0xcd, 0xd3};
    constexpr static color_wrapper_t rose_300 = {0xfd, 0xa4, 0xaf};
    constexpr static color_wrapper_t rose_400 = {0xfb, 0x71, 0x85};
    constexpr static color_wrapper_t rose_500 = {0xf4, 0x3f, 0x5e};
    constexpr static color_wrapper_t rose_600 = {0xe1, 0x1d, 0x48};
    constexpr static color_wrapper_t rose_700 = {0xbe, 0x12, 0x3c};
    constexpr static color_wrapper_t rose_800 = {0x9f, 0x12, 0x39};
    constexpr static color_wrapper_t rose_900 = {0x88, 0x13, 0x37};
    constexpr static color_wrapper_t rose_950 = {0x4c, 0x05, 0x19};

    constexpr static color_wrapper_t border_black = {0x00, 0x00, 0x00};
    constexpr static color_wrapper_t border_white = {0xff, 0xff, 0xff};

    constexpr static color_wrapper_t border_gray_100 = {0xf7, 0xfa, 0xfc};
    constexpr static color_wrapper_t border_gray_200 = {0xed, 0xf2, 0xf7};
    constexpr static color_wrapper_t border_gray_300 = {0xe2, 0xe8, 0xf0};
    constexpr static color_wrapper_t border_gray_400 = {0xcb, 0xd5, 0xe0};
    constexpr static color_wrapper_t border_gray_500 = {0xa0, 0xae, 0xc0};
    constexpr static color_wrapper_t border_gray_600 = {0x71, 0x80, 0x96};
    constexpr static color_wrapper_t border_gray_700 = {0x4a, 0x55, 0x68};
    constexpr static color_wrapper_t border_gray_800 = {0x2d, 0x37, 0x48};
    constexpr static color_wrapper_t border_gray_900 = {0x1a, 0x20, 0x2c};
    constexpr static color_wrapper_t border_red_100 = {0xff, 0xf5, 0xf5};
    constexpr static color_wrapper_t border_red_200 = {0xfe, 0xd7, 0xd7};
    constexpr static color_wrapper_t border_red_300 = {0xfe, 0xb2, 0xb2};
    constexpr static color_wrapper_t border_red_400 = {0xfc, 0x81, 0x81};
    constexpr static color_wrapper_t border_red_500 = {0xf5, 0x65, 0x65};
    constexpr static color_wrapper_t border_red_600 = {0xe5, 0x3e, 0x3e};
    constexpr static color_wrapper_t border_red_700 = {0xc5, 0x30, 0x30};
    constexpr static color_wrapper_t border_red_800 = {0x9b, 0x2c, 0x2c};
    constexpr static color_wrapper_t border_red_900 = {0x74, 0x2a, 0x2a};

    constexpr static color_wrapper_t border_orange_100 = {0xff, 0xfa, 0xf0};
    constexpr static color_wrapper_t border_orange_200 = {0xfe, 0xeb, 0xc8};
    constexpr static color_wrapper_t border_orange_300 = {0xfb, 0xd3, 0x8d};
    constexpr static color_wrapper_t border_orange_400 = {0xf6, 0xad, 0x55};
    constexpr static color_wrapper_t border_orange_500 = {0xed, 0x89, 0x36};
    constexpr static color_wrapper_t border_orange_600 = {0xdd, 0x6b, 0x20};
    constexpr static color_wrapper_t border_orange_700 = {0xc0, 0x56, 0x21};
    constexpr static color_wrapper_t border_orange_800 = {0x9c, 0x42, 0x21};
    constexpr static color_wrapper_t border_orange_900 = {0x7b, 0x34, 0x1e};

    constexpr static color_wrapper_t border_yellow_100 = {0xff, 0xff, 0xf0};
    constexpr static color_wrapper_t border_yellow_200 = {0xfe, 0xfc, 0xbf};
    constexpr static color_wrapper_t border_yellow_300 = {0xfa, 0xf0, 0x89};
    constexpr static color_wrapper_t border_yellow_400 = {0xf6, 0xe0, 0x5e};
    constexpr static color_wrapper_t border_yellow_500 = {0xec, 0xc9, 0x4b};
    constexpr static color_wrapper_t border_yellow_600 = {0xd6, 0x9e, 0x2e};
    constexpr static color_wrapper_t border_yellow_700 = {0xb7, 0x79, 0x1f};
    constexpr static color_wrapper_t border_yellow_800 = {0x97, 0x5a, 0x16};
    constexpr static color_wrapper_t border_yellow_900 = {0x74, 0x42, 0x10};

    constexpr static color_wrapper_t border_green_100 = {0xf0, 0xff, 0xf4};
    constexpr static color_wrapper_t border_green_200 = {0xc6, 0xf6, 0xd5};
    constexpr static color_wrapper_t border_green_300 = {0x9a, 0xe6, 0xb4};
    constexpr static color_wrapper_t border_green_400 = {0x68, 0xd3, 0x91};
    constexpr static color_wrapper_t border_green_500 = {0x48, 0xbb, 0x78};
    constexpr static color_wrapper_t border_green_600 = {0x38, 0xa1, 0x69};
    constexpr static color_wrapper_t border_green_700 = {0x2f, 0x85, 0x5a};
    constexpr static color_wrapper_t border_green_800 = {0x27, 0x67, 0x49};
    constexpr static color_wrapper_t border_green_900 = {0x22, 0x54, 0x3d};

    constexpr static color_wrapper_t border_teal_100 = {0xe6, 0xff, 0xfa};
    constexpr static color_wrapper_t border_teal_200 = {0xb2, 0xf5, 0xea};
    constexpr static color_wrapper_t border_teal_300 = {0x81, 0xe6, 0xd9};
    constexpr static color_wrapper_t border_teal_400 = {0x4f, 0xd1, 0xc5};
    constexpr static color_wrapper_t border_teal_500 = {0x38, 0xb2, 0xac};
    constexpr static color_wrapper_t border_teal_600 = {0x31, 0x97, 0x95};
    constexpr static color_wrapper_t border_teal_700 = {0x2c, 0x7a, 0x7b};
    constexpr static color_wrapper_t border_teal_800 = {0x28, 0x5e, 0x61};
    constexpr static color_wrapper_t border_teal_900 = {0x23, 0x4e, 0x52};

    constexpr static color_wrapper_t border_blue_100 = {0xeb, 0xf8, 0xff};
    constexpr static color_wrapper_t border_blue_200 = {0xbe, 0xe3, 0xf8};
    constexpr static color_wrapper_t border_blue_300 = {0x90, 0xcd, 0xf4};
    constexpr static color_wrapper_t border_blue_400 = {0x63, 0xb3, 0xed};
    constexpr static color_wrapper_t border_blue_500 = {0x42, 0x99, 0xe1};
    constexpr static color_wrapper_t border_blue_600 = {0x31, 0x82, 0xce};
    constexpr static color_wrapper_t border_blue_700 = {0x2b, 0x6c, 0xb0};
    constexpr static color_wrapper_t border_blue_800 = {0x2c, 0x52, 0x82};
    constexpr static color_wrapper_t border_blue_900 = {0x2a, 0x43, 0x65};

    constexpr static color_wrapper_t border_indigo_100 = {0xeb, 0xf4, 0xff};
    constexpr static color_wrapper_t border_indigo_200 = {0xc3, 0xda, 0xfe};
    constexpr static color_wrapper_t border_indigo_300 = {0xa3, 0xbf, 0xfa};
    constexpr static color_wrapper_t border_indigo_400 = {0x7f, 0x9c, 0xf5};
    constexpr static color_wrapper_t border_indigo_500 = {0x66, 0x7e, 0xea};
    constexpr static color_wrapper_t border_indigo_600 = {0x5a, 0x67, 0xd8};
    constexpr static color_wrapper_t border_indigo_700 = {0x4c, 0x51, 0xbf};
    constexpr static color_wrapper_t border_indigo_800 = {0x43, 0x41, 0x90};
    constexpr static color_wrapper_t border_indigo_900 = {0x3c, 0x36, 0x6b};

    constexpr static color_wrapper_t border_purple_100 = {0xfa, 0xf5, 0xff};
    constexpr static color_wrapper_t border_purple_200 = {0xe9, 0xd8, 0xfd};
    constexpr static color_wrapper_t border_purple_300 = {0xd6, 0xbc, 0xfa};
    constexpr static color_wrapper_t border_purple_400 = {0xb7, 0x94, 0xf4};
    constexpr static color_wrapper_t border_purple_500 = {0x9f, 0x7a, 0xea};
    constexpr static color_wrapper_t border_purple_600 = {0x80, 0x5a, 0xd5};
    constexpr static color_wrapper_t border_purple_700 = {0x6b, 0x46, 0xc1};
    constexpr static color_wrapper_t border_purple_800 = {0x55, 0x3c, 0x9a};
    constexpr static color_wrapper_t border_purple_900 = {0x44, 0x33, 0x7a};

    constexpr static color_wrapper_t border_pink_100 = {0xff, 0xf5, 0xf7};
    constexpr static color_wrapper_t border_pink_200 = {0xfe, 0xd7, 0xe2};
    constexpr static color_wrapper_t border_pink_300 = {0xfb, 0xb6, 0xce};
    constexpr static color_wrapper_t border_pink_400 = {0xf6, 0x87, 0xb3};
    constexpr static color_wrapper_t border_pink_500 = {0xed, 0x64, 0xa6};
    constexpr static color_wrapper_t border_pink_600 = {0xd5, 0x3f, 0x8c};
    constexpr static color_wrapper_t border_pink_700 = {0xb8, 0x32, 0x80};
    constexpr static color_wrapper_t border_pink_800 = {0x97, 0x26, 0x6d};
    constexpr static color_wrapper_t border_pink_900 = {0x70, 0x24, 0x59};

    constexpr static color_wrapper_t accent = {0xd2, 0x73, 0x8a};
    constexpr static color_wrapper_t accent_light = {0xce, 0x95, 0xa3};
    constexpr static color_wrapper_t accent_dark = {0xc5, 0x6d, 0x82};
    constexpr static color_wrapper_t accent_darker = {0xa5, 0x59, 0x6b};
    constexpr static color_wrapper_t accent_border = {0x43, 0x4e, 0x5f};

}  // namespace gui::colors

#undef MAKE_COLOR
