//
// Created by 王泽远 on 2022/4/14.
//

#ifndef EMBREE_RAYTRACER_BXDF_H
#define EMBREE_RAYTRACER_BXDF_H

#include "core/utility.h"
#include "core/color.h"

NAMESPACE_BEGIN(ert)
    enum class BxDFType {
        REFLECTION = 1 << 0,
        TRANSMISSION = 1 << 1,
        DIFFUSE = 1 << 2,
        GLOSSY = 1 << 3,
        SPECULAR = 1 << 4,
    };

    class BxDF {
    public:
        explicit BxDF(BxDFType type) : m_type(type) {};

        virtual color f(const vec3 &wo, const vec3 &wi) const = 0;

        virtual color sample_f(const vec3 &wo, vec3 &wi,
                               const point3 &sample, ERTFloat &pdf,
                               BxDFType *sampledType = nullptr) const = 0;

    private:
        const BxDFType m_type;
    };

NAMESPACE_END(ert)
#endif //EMBREE_RAYTRACER_BXDF_H
