//
// Created by 王泽远 on 2022/1/26.
//

#ifndef MAIN_CPP_POLY_MESH_H
#define MAIN_CPP_POLY_MESH_H

#include "fast_obj.h"
#include <string>
#include "geometry.h"
#include "rend/scene.h"


NAMESPACE_BEGIN(ert)
    NAMESPACE_BEGIN(geo)
        class poly_mesh : public geometry {
        public:
            poly_mesh() = delete;

            explicit poly_mesh(const std::string &path,
                               vec3 scale = vec3{1.0f},
                               vec3 transform = vec3{0.0f}) : m_scale(scale), m_transform(transform) {
                m_path = path;
            };

            void set_mat(const std::shared_ptr<material> &material) {
                m_mat = material;
            }

            void loadAndAttachObj(std::shared_ptr<scene> s, std::string path);

            void addTo(std::shared_ptr<scene> s) override;

            ERTFloat area() const override {
                return 0;
            }

            point3 random_point() const override {
                return {};
            }

            bool hit(RTCRayHit &rayHit) override {
                return false;
            };

            ~poly_mesh() {
                fast_obj_destroy(mesh);
            }

        private:
            std::string m_path;
            fastObjMesh *mesh{};

            vec3 m_scale;
            vec3 m_transform;

            std::shared_ptr<material> m_mat;

            void loadAndAttachGrp(const fastObjGroup &grp, std::shared_ptr<scene> s);
        };
    NAMESPACE_END(geo)
NAMESPACE_END(ert)
#endif //MAIN_CPP_POLY_MESH_H
