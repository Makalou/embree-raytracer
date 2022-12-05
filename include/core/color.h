#pragma once

#include "core/vec3.h"
#include "core/utility.h"

NAMESPACE_BEGIN(ert)
    NAMESPACE_BEGIN(img)
        extern const float aspect_ratio;
        extern const int image_width;
        extern const int image_height;
        extern const int channel_num;
        extern int pixel_nums;

        extern uint8_t *pixels;

        void write_color(const color &pixel_color, int i, int j);

    NAMESPACE_END(img)
NAMESPACE_END(ert)