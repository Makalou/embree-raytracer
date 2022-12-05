//
// Created by 王泽远 on 2022/4/14.
//
#include "core/color.h"
#include "core/utility.h"

NAMESPACE_BEGIN(ert)
    NAMESPACE_BEGIN(img)
        const float aspect_ratio = 16.0 / 9.0;
        const int image_width = 1920;
        const int image_height = static_cast<int>(image_width / aspect_ratio);
        const int channel_num = 3;

        int pixel_nums = ert::img::image_width * ert::img::image_height;

        uint8_t *pixels = new uint8_t[image_width * image_height * channel_num];

        void write_color(const color &pixel_color, const int i, const int j) {
            int index = ((img::image_height - 1 - j) * img::image_width + i) * 3;

            int ir = static_cast<int>(256 * clamp(pixel_color.x(), 0.0, 0.999));
            int ig = static_cast<int>(256 * clamp(pixel_color.y(), 0.0, 0.999));
            int ib = static_cast<int>(256 * clamp(pixel_color.z(), 0.0, 0.999));

            pixels[index] = ir;
            pixels[index + 1] = ig;
            pixels[index + 2] = ib;
        }
    NAMESPACE_END(img)
NAMESPACE_END(ert)
