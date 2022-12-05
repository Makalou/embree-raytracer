//
// Created by 王泽远 on 2022/4/14.
//

#ifndef EMBREE_RAYTRACER_SHADER_H
#define EMBREE_RAYTRACER_SHADER_H

#include "core/utility.h"
#include "core/vec3.h"

NAMESPACE_BEGIN(ert)
    class shader {
    public:
        virtual color shade() = 0;
    };
NAMESPACE_END(ert)
#endif //EMBREE_RAYTRACER_SHADER_H
