//
// Created by 王泽远 on 2022/4/14.
//

#ifndef EMBREE_RAYTRACER_GEOMETRY_H
#define EMBREE_RAYTRACER_GEOMETRY_H

#include "core/utility.h"
#include "core/vec3.h"
#include "embree3/rtcore.h"

NAMESPACE_BEGIN(ert)
    class scene;

    class geometry {
    public:
        virtual void addTo(std::shared_ptr<scene> s) = 0;

        virtual point3 random_point() const = 0;

        virtual ERTFloat area() const = 0;

        virtual bool hit(RTCRayHit &rayHit) = 0;

    protected:
        size_t id1;
        size_t id2;
        std::shared_ptr<scene> mp_scene;
    };
NAMESPACE_END(ert)
#endif //EMBREE_RAYTRACER_GEOMETRY_H
