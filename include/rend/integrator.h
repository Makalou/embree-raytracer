//
// Created by 王泽远 on 2022/4/21.
//

#ifndef EMBREE_RAYTRACER_INTEGRATOR_H
#define EMBREE_RAYTRACER_INTEGRATOR_H

#include "core/utility.h"

NAMESPACE_BEGIN(ert)
    class integrator {
    public:
        virtual void L() = 0;
    };

    class pathIntegrator : public integrator {

    };

    class NEE : public integrator {

    };
NAMESPACE_END(ert)

#endif //EMBREE_RAYTRACER_INTEGRATOR_H
