//
// Created by 王泽远 on 2022/4/14.
//
#include "rend/scene.h"

#include <utility>

NAMESPACE_BEGIN(ert)
    scene::scene() {
        m_device = rtcNewDevice(nullptr);
        m_scene = rtcNewScene(m_device);
        m_lights = rtcNewScene(m_device);
        rtcInitIntersectContext(&context);
    }

    scene::~scene() {
        //rtcReleaseScene(m_scene);
        rtcReleaseDevice(m_device);
    }

    void scene::add(geometry &&geom) {
        geom.addTo(shared_from_this());
    }

    void scene::attachCamera(std::shared_ptr<camera> cam) {
        mp_cam = std::move(cam);
    }

    void scene::commit() const {
        spdlog::info("materials: size {} load factor:{}", m_materials.size(), m_materials.load_factor());
        spdlog::info("material_table:{}", m_mat_table.size());
        rtcCommitScene(m_scene);
        rtcCommitScene(m_lights);
    }

    void scene::addLight(const std::shared_ptr<geometry> &l) {
        light = l;
        l->addTo(shared_from_this());
    }

    std::shared_ptr<sampler> scene::sample_lights(const point3 &p) const {
        return std::make_shared<geo_sampler>(light, p);
    }
NAMESPACE_END(ert)
