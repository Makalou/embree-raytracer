//
// Created by 王泽远 on 2022/4/14.
//

#ifndef EMBREE_RAYTRACER_HIT_RECORD_H
#define EMBREE_RAYTRACER_HIT_RECORD_H

#include "embree3/rtcore.h"

NAMESPACE_BEGIN(ert)

    class material;

    class hit_record {
    public:
        point3 p;
        vec3 normal;
        shared_ptr<material> mat_ptr;
        ERTFloat t;
        ERTFloat u;
        ERTFloat v;
        bool front_face{};

        explicit hit_record(const RTCRayHit &rayHit) {
            p = vec3{rayHit.ray.org_x, rayHit.ray.org_y, rayHit.ray.org_z} +
                rayHit.ray.tfar * vec3{rayHit.ray.dir_x, rayHit.ray.dir_y, rayHit.ray.dir_z};
            t = rayHit.ray.tfar;
            u = rayHit.hit.u;
            v = rayHit.hit.v;
        }

        void set_face_normal(const RTCRay &r, const vec3 &outward_normal) {
            front_face = dot({r.dir_x, r.dir_y, r.dir_z}, outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
    };
NAMESPACE_END(ert)
#endif //EMBREE_RAYTRACER_HIT_RECORD_H
