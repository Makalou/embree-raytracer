//
// Created by 王泽远 on 2022/4/14.
//

#ifndef EMBREE_RAYTRACER_SCENE_H
#define EMBREE_RAYTRACER_SCENE_H

#include "core/utility.h"
#include "embree3/rtcore.h"
#include <unordered_map>
#include <unordered_set>
#include "rend/material.h"
#include "geom/geometry.h"
#include "rend/camera.h"
#include "spdlog/spdlog.h"
#include "core/sampler.h"

NAMESPACE_BEGIN(ert)
    struct sceneMatHash {
    public:
        size_t operator()(const shared_ptr<material> &p) const {
            return p->hash();
        }
    };

    struct sceneMatEqual {
    public:
        bool operator()(const shared_ptr<material> &lp, const shared_ptr<material> &rp) const {
            return *lp == *rp;
        }
    };

    class geometry;

    class scene : public std::enable_shared_from_this<scene> {
    public:
        scene();

        ~scene();

        scene(const scene &) = delete;

        scene &operator=(const scene &) = delete;

        void add(geometry &&geom);

        void addLight(const std::shared_ptr<geometry> &);

        std::shared_ptr<sampler> sample_lights(const point3 &) const;

        void attachCamera(std::shared_ptr<camera> cam);

        void commit() const;

        bool hit(RTCRayHit &rayHit) {
            rtcIntersect1(m_scene, &context, &rayHit);
            return rayHit.hit.geomID != RTC_INVALID_GEOMETRY_ID;
        }

        std::shared_ptr<material> get_material(unsigned int id) {
            return m_mat_table[id];
        }

    public:
        RTCScene m_scene;
        RTCScene m_lights;
        RTCDevice m_device;

        std::shared_ptr<camera> mp_cam;
        std::unordered_set<std::shared_ptr<material>, sceneMatHash, sceneMatEqual> m_materials;
        //std::unordered_map<unsigned int,std::shared_ptr<material>> m_mat_table;
        std::vector<std::shared_ptr<material>> m_mat_table;
        std::shared_ptr<geometry> light;
        RTCIntersectContext context{};
    };

NAMESPACE_END(ert)
#endif //EMBREE_RAYTRACER_SCENE_H
