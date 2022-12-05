//
// Created by 王泽远 on 2022/4/20.
//

#ifndef EMBREE_RAYTRACER_SAMPLER_H
#define EMBREE_RAYTRACER_SAMPLER_H

#include <utility>

#include "utility.h"
#include "vec3.h"
#include "geom/geometry.h"

NAMESPACE_BEGIN(ert)
    class sampler {
    public:
        virtual double pdf(const vec3 &direction) const = 0;

        virtual vec3 generate() const = 0;
    };

    class onb {
    public:
        onb() {}

        inline vec3 operator[](int i) const { return axis[i]; }

        vec3 u() const { return axis[0]; }

        vec3 v() const { return axis[1]; }

        vec3 w() const { return axis[2]; }

        vec3 local(ERTFloat a, ERTFloat b, ERTFloat c) const {
            return a * u() + b * v() + c * w();
        }

        vec3 local(const vec3 &a) const {
            return a.x() * u() + a.y() * v() + a.z() * w();
        }

        void build_from_w(const vec3 &n) {
            axis[2] = unit_vector(n);
            vec3 a = (fabs(w().x()) > 0.9) ? vec3(0, 1, 0) : vec3(1, 0, 0);
            axis[1] = unit_vector(cross(w(), a));
            axis[0] = cross(w(), v());
        }

    public:
        vec3 axis[3];
    };

    class cosine_sampler : public sampler {
    public:
        explicit cosine_sampler(const vec3 &w) {
            uvw.build_from_w(w);
        }

        double pdf(const vec3 &direction) const override {
            auto cosine = dot(unit_vector(direction), uvw.w());
            return (cosine <= 0) ? 0 : cosine / pi;
        }

        vec3 generate() const override {
            return uvw.local(random_cosine_direction());
        }

    private:
        static vec3 random_cosine_direction() {
            auto r1 = random_FLOAT();
            auto r2 = random_FLOAT();
            auto z = sqrt(1 - r2);

            auto phi = 2 * pi * r1;
            auto x = cos(phi) * sqrt(r2);
            auto y = sin(phi) * sqrt(r2);

            return {x, y, z};
        }

    public:
        onb uvw;
    };

    class geo_sampler : public sampler {
    public:
        geo_sampler(shared_ptr<geometry> p, const point3 &origin) : ptr(std::move(p)), o(origin) {}

        double pdf(const vec3 &direction) const {

            RTCRayHit rayHit{};
            rayHit.ray.org_x = o.x();
            rayHit.ray.org_y = o.y();
            rayHit.ray.org_z = o.z();
            rayHit.ray.dir_x = direction.x();
            rayHit.ray.dir_y = direction.y();
            rayHit.ray.dir_z = direction.z();
            rayHit.ray.tnear = 0;
            rayHit.ray.tfar = std::numeric_limits<ERTFloat>::infinity();
            rayHit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

            if (!ptr->hit(rayHit))
                return 0;

            auto area = ptr->area();
            double distance_squared = rayHit.ray.tfar * rayHit.ray.tfar;
            auto cosine = fabs(dot(direction, unit_vector(vec3{rayHit.hit.Ng_x, rayHit.hit.Ng_y, rayHit.hit.Ng_z})));

            return distance_squared / (cosine * area);
        }

        vec3 generate() const {
            return unit_vector(ptr->random_point() - o);
        }

    private:
        point3 o;
        shared_ptr<geometry> ptr;
    };

    class mixture_sampler : sampler {
    public:
        mixture_sampler(std::shared_ptr<sampler> p0, std::shared_ptr<sampler> p1) {
            samplers[0] = std::move(p0);
            samplers[1] = std::move(p1);
        }

        mixture_sampler() {};

        double pdf(const vec3 &direction) const override {
            return w * samplers[0]->pdf(direction) + (1 - w) * samplers[1]->pdf(direction);
        }

        vec3 generate() const override {
            if (random_FLOAT(0, 1) < w)
                return samplers[0]->generate();
            else
                return samplers[1]->generate();
        }

    private:
        std::shared_ptr<sampler> samplers[2];
        double w = 0.7;
    };
NAMESPACE_END(ert)

#endif //EMBREE_RAYTRACER_SAMPLER_H
