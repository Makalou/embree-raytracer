#pragma once

#include "core/utility.h"
#include "core/vec3.h"
#include "embree3/rtcore.h"

NAMESPACE_BEGIN(ert)
    class camera {
    public:
        camera(point3 lookfrom, point3 lookat, double vfov = 60.0, double aspect_ratio = 16.0 / 9.0,
               double aperture = 0.0);

        RTCRay get_ray(ERTFloat s, ERTFloat t) const;

        void look_from(point3 point);

        color get_bg(const vec3 &dir) const {
            return bg;
        }

    public:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;

        ERTFloat viewport_height;
        ERTFloat viewport_width;
        ERTFloat focal_length;
        ERTFloat aperture;

        vec3 lookfrom;
        vec3 lookat;
        vec3 u, v, w;

        color bg = ert::color(0.70, 0.80, 1.00);

        void update_vectors();
    };
NAMESPACE_END(ert)