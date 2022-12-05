//
// Created by 王泽远 on 2022/4/14.
//
#include "renderer.h"
#include "viewer.h"
#include "core/color.h"
#include "indicators/indicators.hpp"
#include "tbb/tbb.h"
#include <future>
#include <utility>

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "photon_mapping.h"

#define HIT_SCENE(rayhit) (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID)

NAMESPACE_BEGIN(ert)

    void renderer::renderTo(std::string_view file_name, bool gui_enable) {
        rendering_progress = 0;
        const std::string suffix = ".png";

        auto ray_tracing_task = [&] {

            this->mp_scene->mp_cam->origin += vec3{0, 0.02, 0};
            spdlog::info("Start rendering...\n");
            //indicator
            indicators::show_console_cursor(false);
            indicators::BlockProgressBar bar{
                    indicators::option::BarWidth{50},
                    indicators::option::Start{"["},
                    indicators::option::End{"]"},
                    indicators::option::PostfixText{"Rendering "},
                    indicators::option::ForegroundColor{indicators::Color::white},
                    indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}},
                    indicators::option::ShowElapsedTime{true},
                    indicators::option::ShowRemainingTime{true},
                    indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
            };

            std::thread t([&] {
                // Update bar state
                while (true) {
                    bar.set_progress(float(rendering_progress) / float(img::pixel_nums) * 101.f);
                    if (bar.is_completed())
                        break;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                // Show cursor
                indicators::show_console_cursor(true);
            });
            {
                tbb::blocked_range2d<int, int> range2D(0, ert::img::image_width, 0, ert::img::image_height);
                auto map2D = [&](const tbb::blocked_range2d<int, int> &range2D) {
                    for (int i = range2D.rows().begin(); i < range2D.rows().end(); ++i) {
                        for (int j = range2D.cols().begin(); j < range2D.cols().end(); ++j) {
                            render_pixel(i, j);
                        }
                    }
                };

                tbb::parallel_for(range2D, map2D);

                spdlog::info("Done.\n");

            }
            t.join();
        };

        auto result = std::async(ray_tracing_task);

        if (gui_enable) {
            auto v = viewer::viewer::create(file_name);
            //window loop
            if (v) v->mainloop();
            else spdlog::error("ERROR! Create viewer failed!");
        }

        result.wait();
        stbi_write_png((std::string(file_name) + suffix).c_str(), ert::img::image_width, ert::img::image_height,
                       ert::img::channel_num, ert::img::pixels, ert::img::image_width * ert::img::channel_num);
        //delete[] ert::img::pixels;
    }

    void renderer::attachScene(std::shared_ptr<scene> scene) {
        mp_scene = std::move(scene);
    }

    void renderer::render_line(int j) {
        tbb::blocked_range<int> range(0, ert::img::image_width);
        auto map = [&](const tbb::blocked_range<int> &range) {
            for (int i = range.begin(); i < range.end(); ++i) {
                rendering_progress++;
                render_pixel(i, j);
            }
        };
        tbb::parallel_for(range, map);
    }

    void renderer::render_tile(int i, int j) {
        for (int ii = 0; ii < 4; ii++) {
            for (int jj = 0; jj < 4; jj++) {
                render_pixel(i + ii, j + jj);
            }
        }
    }

    void renderer::render_pixel(int i, int j) {
        rendering_progress++;
        img::write_color(color{random_FLOAT()}, i, j);
        return;
        std::array<RTCRay, spp> rays{};
        tbb::blocked_range<int> range(0, rays.size());

        std::generate(rays.begin(), rays.end(), [&] {
            auto u = ERTFloat(static_cast<ERTFloat>(i) + ert::random_FLOAT()) / ERTFloat(ert::img::image_width - 1);
            auto v = ERTFloat(static_cast<ERTFloat>(j) + ert::random_FLOAT()) / ERTFloat(ert::img::image_height - 1);
            return mp_scene->mp_cam->get_ray(u, v);
        });

        std::array<ert::color, spp> pixel_colors;

        auto trace_ray = [&](const tbb::blocked_range<int> &range) {
            for (int i = range.begin(); i < range.end(); ++i) {
                pixel_colors[i] = L(rays[i], 0);
            }
        };

        tbb::parallel_for(range, trace_ray);

        color pixel_color{};

        for (auto &p: pixel_colors) {
            pixel_color += p;
        }

        {
            auto r = pixel_color.x();
            auto g = pixel_color.y();
            auto b = pixel_color.z();

            if (r != r) r = 0.0;
            if (g != g) g = 0.0;
            if (b != b) b = 0.0;

            auto scale = 1.0 / spp;
            pixel_color.e[0] = sqrt(r * scale);
            pixel_color.e[1] = sqrt(g * scale);
            pixel_color.e[2] = sqrt(b * scale);
        }

        img::write_color(pixel_color, i, j);
    }

    color renderer::L(RTCRay &ray, int depth) {
        if (random_FLOAT() <= q) {
            return {0, 0, 0};
        }
        RTCRayHit rayHit{};
        rayHit.ray = ray;
        rayHit.ray.tnear = 0;
        rayHit.ray.tfar = std::numeric_limits<ERTFloat>::infinity();
        rayHit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
        if (!mp_scene->hit(rayHit)) {
            return mp_scene->mp_cam->get_bg({ray.dir_x, ray.dir_y, ray.dir_z}) / (1 - q);
        } else {
            RTCRay scattered{};
            //ert::color attenuation;
            ert::hit_record rec(rayHit);

            rec.mat_ptr = mp_scene->get_material(rayHit.hit.geomID);

            if (rec.mat_ptr == nullptr) {
                spdlog::error("NULL MATERIAL FOUND!");
            }

            ert::color emitted = rec.mat_ptr->emitted(rayHit.hit.u, rayHit.hit.v, rec.p);

            rec.normal.e[0] = rayHit.hit.Ng_x;
            rec.normal.e[1] = rayHit.hit.Ng_y;
            rec.normal.e[2] = rayHit.hit.Ng_z;

            rec.normal = unit_vector(rec.normal);

            //return {rec.normal.x(),rec.normal.y(),rec.normal.z()};

            ert::color albedo;

            if (!rec.mat_ptr->scatter(rayHit.ray, rec, albedo, scattered)) {
                return emitted / (1 - q);
            }


            if (rec.mat_ptr->is_specular) {
                return emitted + albedo * L(scattered, depth + 1) / (1 - q);
            }

            //albedo = {rec.normal.x(),rec.normal.y(),rec.normal.z()};
            auto p0 = std::make_shared<cosine_sampler>(rec.normal);
            auto p1 = mp_scene->sample_lights(rec.p);
            auto p_ms = mixture_sampler(p0, p0);

            auto some = p_ms.generate();

            scattered.dir_x = some.x();
            scattered.dir_y = some.y();
            scattered.dir_z = some.z();

            auto brdf = rec.mat_ptr->scattering_pdf(rayHit.ray, rec, scattered);
            auto l = L(scattered, depth + 1);
            auto fx = (albedo * brdf * l) / p_ms.pdf(some);

            return (emitted + fx) / (1 - q);
        }
    }

NAMESPACE_END(ert)

