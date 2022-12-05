//
// Created by 王泽远 on 2022/4/14.
//

#ifndef EMBREE_RAYTRACER_RENDERER_H
#define EMBREE_RAYTRACER_RENDERER_H

#include "core/utility.h"
#include "scene.h"

NAMESPACE_BEGIN(ert)
    class renderer {
    public:
        void attachScene(std::shared_ptr<scene> scene);

        void renderTo(std::string_view file_name, bool gui_enable = true);

    private:
        std::shared_ptr<scene> mp_scene;
        static constexpr int spp = 100;
        static constexpr double q = 0.2;
    private:
        void render_line(int j);

        void render_pixel(int i, int j);

        void render_tile(int i, int j);

        color L(RTCRay &ray, int depth);

        std::atomic<int> rendering_progress;
    };
NAMESPACE_END(ert)
#endif //EMBREE_RAYTRACER_RENDERER_H
