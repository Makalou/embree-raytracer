//
// Created by 王泽远 on 2022/4/14.
//

#include "camera.h"
//#define MEMCPY

NAMESPACE_BEGIN(ert)
    camera::camera(point3 lookfrom, point3 lookat,
                   double vfov, double aspect_ratio,
                   double aperture) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);

        viewport_height = 2.0 * h;
        viewport_width = aspect_ratio * viewport_height;
        focal_length = 1.0;
        this->aperture = aperture;

        this->lookfrom = lookfrom;
        this->lookat = lookat;

        update_vectors();
    }

    RTCRay camera::get_ray(ERTFloat s, ERTFloat t) const {
        RTCRay ray{};
        ray.tfar = std::numeric_limits<float>::infinity();
        ray.tnear = 0.0f;

        if (aperture == 0) {
#ifdef MEMCPY
            memcpy(&ray.org_x,origin.e,96);
#else
            ray.org_x = origin.e[0];
            ray.org_y = origin.e[1];
            ray.org_z = origin.e[2];
#endif

            auto direction = lower_left_corner + s * horizontal + t * vertical - origin;
#ifdef MEMCPY
            memcpy(&ray.dir_x,direction.e,96);
#else
            ray.dir_x = direction.e[0];
            ray.dir_y = direction.e[1];
            ray.dir_z = direction.e[2];
#endif
        } else {
            ERTFloat lens_radius = aperture / 2;
            vec3 rd = lens_radius * random_in_unit_disk<ERTFloat>();
            vec3 offset = u * rd.x() + v * rd.y();

            ray.org_x = origin.e[0] + offset.e[0];
            ray.org_y = origin.e[1] + offset.e[1];
            ray.org_z = origin.e[2] + offset.e[2];

            auto direction = lower_left_corner + s * horizontal + t * vertical - origin - offset;

#ifdef MEMCPY
            memcpy(&ray.dir_x,direction.e,96);
#else
            ray.dir_x = direction.e[0];
            ray.dir_y = direction.e[0];
            ray.dir_z = direction.e[0];
#endif
        }
        return ray;
    }

    void camera::look_from(point3 point) {
        this->lookfrom = point;
        update_vectors();
    }

    void camera::update_vectors() {
        w = unit_vector(lookfrom - lookat);
        auto vup = vec3(0, 1, 0);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        auto focus_dist = (lookfrom - lookat).length();

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - focus_dist * w;
    }
NAMESPACE_END(ert)

