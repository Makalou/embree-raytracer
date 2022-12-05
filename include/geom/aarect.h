//
// Created by 王泽远 on 2022/1/20.
//

#ifndef MAIN_CPP_AARECT_H
#define MAIN_CPP_AARECT_H

#include <utility>

#include "geom/geometry.h"
#include "core/utility.h"
#include "rend/material.h"
#include "rend/scene.h"

NAMESPACE_BEGIN(ert)
    NAMESPACE_BEGIN(geo)
        enum class Face {
            FONT,
            BACK
        };

        class xy_rect : public geometry {
        public:
            xy_rect() = default;

            xy_rect(ERTFloat _x0, ERTFloat _x1, ERTFloat _y0, ERTFloat _y1, ERTFloat _k,
                    shared_ptr<material> mat, Face face = Face::FONT)
                    : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(std::move(mat)) {
                if (face == Face::BACK) {
                    idx1 = 6;
                    idx2 = 7;
                    idx3 = 0;
                    idx4 = 1;
                }
            };

            void addTo(std::shared_ptr<scene> s) override;

            point3 random_point() const override {
                return {random_FLOAT(x0, x1), random_FLOAT(y0, y1), k};
            }

            ERTFloat area() const override {
                return (x1 - x0) * (y1 - y0);
            }

            bool hit(RTCRayHit &rayHit) override {
                return false;
            }

        public:
            shared_ptr<material> mp;
            ERTFloat x0{}, x1{}, y0{}, y1{}, k{};
        private:
            int idx1 = 0;
            int idx2 = 1;
            int idx3 = 6;
            int idx4 = 7;
        };

        class xz_rect : public geometry {
        public:
            xz_rect() = default;

            xz_rect(ERTFloat _x0, ERTFloat _x1, ERTFloat _z0, ERTFloat _z1, ERTFloat _k,
                    shared_ptr<material> mat, Face face = Face::FONT)
                    : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(std::move(mat)) {
                if (face == Face::BACK) {
                    idx1 = 6;
                    idx2 = 8;
                    idx3 = 0;
                    idx4 = 2;
                }
            };

            point3 random_point() const override {
                return {random_FLOAT(x0, x1), k, random_FLOAT(z0, z1)};
            }

            ERTFloat area() const override {
                return (x1 - x0) * (z1 - z0);
            }

            bool hit(RTCRayHit &rayHit) override {
                if (!mp_scene->hit(rayHit))
                    return false;
                return (rayHit.hit.geomID == id1 || rayHit.hit.geomID == id2);
            }

            void addTo(std::shared_ptr<scene> s) override;

        public:
            shared_ptr<material> mp;
            ERTFloat x0{}, x1{}, z0{}, z1{}, k{};
        private:
            int idx1 = 0;
            int idx2 = 2;
            int idx3 = 6;
            int idx4 = 8;
        };

        class yz_rect : public geometry {
        public:
            yz_rect() = default;

            yz_rect(ERTFloat _y0, ERTFloat _y1, ERTFloat _z0, ERTFloat _z1, ERTFloat _k,
                    shared_ptr<material> mat, Face face = Face::FONT)
                    : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(std::move(mat)) {
                if (face == Face::BACK) {
                    idx1 = 7;
                    idx2 = 8;
                    idx3 = 1;
                    idx4 = 2;
                }
            };

            point3 random_point() const override {
                return {k, random_FLOAT(y0, y1), random_FLOAT(z0, z1)};
            }

            ERTFloat area() const override {
                return (z1 - z0) * (y1 - y0);
            }

            bool hit(RTCRayHit &rayHit) override {
                return false;
            }

            void addTo(std::shared_ptr<scene> s) override;

        public:
            shared_ptr<material> mp;
            ERTFloat y0, y1, z0, z1, k;
        private:
            int idx1 = 1;
            int idx2 = 2;
            int idx3 = 7;
            int idx4 = 8;
        };

    NAMESPACE_END(geo)
NAMESPACE_END(ert)
#endif //MAIN_CPP_AARECT_H
