#pragma once

#include <cmath>
#include <iostream>
#include "utility.h"
#include <arm_neon.h>
#include "simd/sse2neon.h"
#include "spdlog/spdlog.h"

NAMESPACE_BEGIN(ert)
    using std::sqrt;

    template<typename T>
    class vec3T {
    public:
        vec3T() : e{0, 0, 0} {}

        vec3T(T e0, T e1, T e2) : e{e0, e1, e2} {}

        explicit vec3T(T d) : e{d, d, d} {}

        T x() const { return e[0]; }

        T y() const { return e[1]; }

        T z() const { return e[2]; }

        vec3T<T> operator-() const { return vec3T<T>{-e[0], -e[1], -e[2]}; }

        T operator[](int i) const { return e[i]; }

        T &operator[](int i) { return e[i]; }

        bool operator==(const vec3T &v) const {
            return e[0] == v.e[0] && e[1] == v.e[1] && e[2] == v.e[2];
        }

        vec3T &operator+=(const vec3T &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3T &operator*=(const T t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3T &operator/=(const T t) {
            return *this *= 1 / t;
        }

        T length() const {
            return sqrt(length_squared());
        }

        T length_squared() const {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }

    public:
        T e[3];

        inline static vec3T random() {
            return {random_FLOAT(), random_FLOAT(), random_FLOAT()};
        }

        inline static vec3T random(T min, T max) {
            return {random_FLOAT(min, max), random_FLOAT(min, max), random_FLOAT(min, max)};
        }

        bool near_zero() const {
            const auto epsillon = 1e-8;
            return (fabs(e[0]) < epsillon) &&
                   (fabs(e[1]) < epsillon) &&
                   (fabs(e[2]) < epsillon);
        }
    };


    using vec3 = vec3T<ERTFloat>;
    using point3 = vec3;
    using color = vec3T<double>;

    template<typename T>
    inline std::ostream &operator<<(std::ostream &out, const vec3T<T> &v) {
        return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
    }

    template<typename T>
    inline vec3T<T> operator+(const vec3T<T> &u, const vec3T<T> &v) {
        return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
    }

    template<typename T>
    inline vec3T<T> operator-(const vec3T<T> &u, const vec3T<T> &v) {
        return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
    }

    template<typename T>
    inline vec3T<T> operator*(const vec3T<T> &u, const vec3T<T> &v) {
        return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
    }

    template<typename T>
    inline vec3T<T> operator*(T t, const vec3T<T> &v) {
        return {t * v.e[0], t * v.e[1], t * v.e[2]};
    }

    template<typename T>
    inline vec3T<T> operator*(const vec3T<T> &v, T t) {
        return t * v;
    }

    template<typename T>
    inline vec3T<T> operator/(vec3T<T> v, T t) {
        return (1 / t) * v;
    }

    template<typename T>
    inline T dot(const vec3T<T> &u, const vec3T<T> &v) {
        return u.e[0] * v.e[0]
               + u.e[1] * v.e[1]
               + u.e[2] * v.e[2];
    }

    template<typename T>
    inline vec3T<T> cross(const vec3T<T> &u, const vec3T<T> &v) {
        return {u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]};
    }

    template<typename T>
    inline vec3T<T> unit_vector(const vec3T<T> &v) {
        return v / v.length();
    }

    template<typename T>
    inline vec3T<T> random_in_unit_sphere() {
        while (true) {
            auto p = vec3T<T>::random(-1, 1);
            if (p.length_squared() >= 1) continue;
            return p;
        }
    }

/*
* Rejection sampling
*/
    template<typename T>
    inline vec3T<T> random_in_unit_disk() {
        while (true) {
            auto x = random_FLOAT(-1, 1);
            auto y = random_FLOAT(-1, 1);
            if (x * x + y * y >= 1) continue;
            return {x, y, 0};
        }
    }

    template<typename T>
    inline vec3T<T> random_in_unit_disk_stratified() {
        const int sqrt_N = 32;
        int i;
        int j;

        while (true) {
            auto x = 2 * ((i + random_FLOAT()) / sqrt_N) - 1;
            auto y = 2 * ((j + random_FLOAT()) / sqrt_N) - 1;
            if (x * x + y * y >= 1) continue;
            return {x, y, 0};
        }
    }

    template<typename T>
    inline vec3T<T> random_unit_vector() {
        return unit_vector(random_in_unit_sphere<T>());
    }

    template<typename T>
    inline vec3T<T> reflect(const vec3T<T> &v, const vec3T<T> &n) {
        return v - (dot(v, n) * 2) * n;
    }

    template<typename T>
    inline vec3T<T> refract(const vec3 &uv, const vec3 &n, T etai_over_etat) {
        T cos_theta = fmin(dot(-uv, n), (T) 1.0);
        vec3T<T> r_out_perp = etai_over_etat * (uv + cos_theta * n);
        vec3T<T> r_out_parallel = -sqrt(fabs(T(1.0) - r_out_perp.length_squared())) * n;

        return r_out_perp + r_out_parallel;
    }

    template<typename T, size_t size>
    inline vec3T<T> sum_vec3(std::array<vec3T<T>, size> &vec3s) {
        auto _size = size;

        while (_size > 1) {
            std::sort(vec3s.begin(), vec3s.begin() + _size);
            int n = 0;
            for (; 2 * n + 1 < _size; ++n) {
                vec3s[n] = vec3s[2 * n] + vec3s[2 * n + 1];
            }
            if (2 * n + 1 == _size) vec3s[n] = vec3s[2 * n];
            _size = (_size + 1) / 2;
        }
        return vec3s[0];
    }

    template<size_t size>
    inline vec3T<double> sum_vec3(const std::array<vec3T<double>, size> &vec3s) {
        vec3T<double> result;
        for (auto &v: vec3s) {
            result += v;
        }
        return result;
    }
NAMESPACE_END(ert)