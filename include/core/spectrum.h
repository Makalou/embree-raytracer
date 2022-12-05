//
// Created by 王泽远 on 2022/4/19.
//

#ifndef EMBREE_RAYTRACER_SPECTRUM_H
#define EMBREE_RAYTRACER_SPECTRUM_H

#include "utility.h"

NAMESPACE_BEGIN(ert)

    class spectrum {

    };

    template<int nCoefficientNums>
    class coefficientSpectrum : spectrum {
    public:
        explicit coefficientSpectrum(ERTFloat f = 0.f) {
            assert(a >= 0.);
            for (int i = 0; i < nCoefficientNums; ++i) {
                c[i] = f;
            }
        }

        bool isBlack() const;

        ERTFloat &operator[](int i) const {
            return c[i];
        }

    private:
        ERTFloat c[nCoefficientNums]{};
    };

    template<int nCoefficientNums>
    class sampledSpectrum : coefficientSpectrum<nCoefficientNums> {
    private:
        static const int sampledLambdaStart = 400;
        static const int sampledLambdaEnd = 700;
        static const int sampledNums = 60;
    };

NAMESPACE_END(ert)

#endif //EMBREE_RAYTRACER_SPECTRUM_H
