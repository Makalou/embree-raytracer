#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <algorithm>

#define NAMESPACE_BEGIN(name) namespace name{
#define NAMESPACE_END(name) }

using ERTFloat = float;
NAMESPACE_BEGIN(ert)
    using std::shared_ptr;
    using std::make_shared;
    using std::sqrt;

    const ERTFloat infinity = std::numeric_limits<double>::infinity();
    const ERTFloat pi = 3.1415926535897932385;

    inline double degrees_to_radians(double degrees) {
        return degrees * pi / 180.0;
    }

    inline ERTFloat random_FLOAT() {
//	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
//    std::random_device dev;
//	static std::mt19937 generator;
//	return distribution(generator);
        return random() / (RAND_MAX + 1.0);
    }

    inline ERTFloat random_FLOAT(ERTFloat min, ERTFloat max) {
        return min + (max - min) * random_FLOAT();
    }

    inline int random_int(int min, int max) {
        // Returns a random integer in [min,max].
        return static_cast<int>(random_FLOAT(min, max + 1));
    }

    inline double clamp(double v, double min, double max) {
        if (v > min) {
            return v < max ? v : max;
        } else {
            return min;
        }
    }

NAMESPACE_END(ert)