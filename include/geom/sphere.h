//
// Created by 王泽远 on 2022/4/15.
//

#ifndef EMBREE_RAYTRACER_SPHERE_H
#define EMBREE_RAYTRACER_SPHERE_H

#include <utility>

#include "geometry.h"
#include "core/vec3.h"
#include "rend/material.h"

NAMESPACE_BEGIN(ert)
    NAMESPACE_BEGIN(geo)
        class sphere : public geometry {
        public:
            sphere() = default;

            sphere(point3 cen, ERTFloat r) : center(cen), radius(r) {};

            sphere(point3 cen, ERTFloat r, shared_ptr<material> m)
                    : center(cen), radius(r), mat_ptr(std::move(m)) {};
        public:
            point3 center;
            ERTFloat radius{};
            shared_ptr<material> mat_ptr;
        private:
            static void get_sphere_uv(const point3 &p, double &u, double &v) {
                auto theta = acos(-p.y());
                auto phi = atan2(-p.z(), p.x()) + pi;

                u = phi / (2 * pi);
                v = theta / pi;
            }
        };
    NAMESPACE_END(geo)
NAMESPACE_END(ert)
#endif //EMBREE_RAYTRACER_SPHERE_H
