#include "ert.h"
#include <memory>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <chrono>
#include <cstdio>

const double x = 0 / 10;
const double y = 1.2 / 10;
const double z = 1.5 / 5;
//const double r = sqrt(x*x+z*z);

//World
void bunny(ert::scene &w);

void debug_norm(ert::scene &w);

void ajax(ert::scene &w);

void livingroom(ert::scene &w, int t);

void wave(ert::scene &w);

auto mirror = std::make_shared<ert::mat::metal>(ert::color{1.0f}, 0);
auto glass = std::make_shared<ert::mat::dielectric>(1.5);
auto matte = std::make_shared<ert::mat::lambertian>(ert::color{0.5f});

int main(int argc, char **argv) {
    FILE *fp = fopen("big.json", "r"); // 非 Windows 平台使用 "r"
    if (fp) {
        char readBuffer[65536];
        rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

        rapidjson::Document d;
        d.ParseStream(is);
    }
    fclose(fp);

    for (int i = 0; i < 1; i++) {
        auto world = std::make_shared<ert::scene>();
        wave(*world);
        world->attachCamera(std::make_shared<ert::camera>(ert::point3(x, y, z), ert::point3(0, 0.1, 0)));

        ert::renderer renderer;
        renderer.attachScene(world);


        std::string name = "scene_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
        renderer.renderTo(name, true);
        spdlog::info("t:{}", i);
    }

    return 0;
}

void bunny(ert::scene &w) {
    // auto checker = std::make_shared<ert::tex::checker_texture>(ert::color(1.0, 1.0, 1.0), ert::color(0.5, 0.5, 0.5));

    //ground
    auto earth_material = std::make_shared<ert::mat::lambertian>(
            std::make_shared<ert::tex::image_texture>("../../res/earthmap.jpeg"));
    auto material1 = std::make_shared<ert::mat::lambertian>(ert::color(0.5, 0.5, 0.7));
    w.add(ert::geo::xz_rect(-0.2, 0.2, -0.2, 0.2, 0.01, earth_material));
    //back
    auto material2 = std::make_shared<ert::mat::lambertian>(ert::color(0.7, 0.5, 0.5));
    w.add(ert::geo::xy_rect(0.2, -0.2, 0.3, 0.01, -0.2, material2));
    //left
    auto material3 = std::make_shared<ert::mat::lambertian>(ert::color(0.5, 0.7, 0.5));
    w.add(ert::geo::yz_rect(0.01, 0.3, -0.2, 0.2, -0.2, material3, ert::geo::Face::BACK));
    //right
    w.add(ert::geo::yz_rect(0.01, 0.3, -0.2, 0.2, 0.2, material3));
    //ceiling
    w.add(ert::geo::xz_rect(-0.2, 0.2, -0.2, 0.2, 0.3, material1, ert::geo::Face::BACK));

    auto light = std::make_shared<ert::mat::diffuse_light>(ert::color(0.70, 0.80, 1.00));

    w.add(ert::geo::xz_rect(-0.15, 0.15, -0.15, 0.15, 0.299, light, ert::geo::Face::BACK));
    //w.add(ert::geo::xy_rect(0.04,-0.04,0.15-0.04,0.15+0.04,-0.199,light));
    //w.add(ert::geo::yz_rect(0.15-0.04,0.15+0.04,-0.04,0.04,-0.199,light, ert::geo::Face::BACK));
    //w.add(ert::geo::yz_rect(0.15-0.04,0.15+0.04,-0.04,0.04,0.199,light));


    w.add(ert::geo::poly_mesh("../../res/models/dragon.obj", ert::vec3{0.05f}, {0.1f, 0.05f, 0.1f}));
    w.add(ert::geo::poly_mesh("../../res/models/bunny.obj", ert::vec3{0.8f}, {-0.09f, 0.0f, -0.09f}));
    w.add(ert::geo::poly_mesh("../../res/models/elephant.obj", ert::vec3{0.05f}, {0.0f, 0.0f, 0.0f}));
    //w.add(ert::geo::poly_mesh("../../res/models/monkey.obj",ert::vec3{0.05f},{0.0f,0.0f,0.0f}));

    w.commit();
}

void debug_norm(ert::scene &w) {
    auto light = std::make_shared<ert::mat::diffuse_light>(ert::color(10, 10, 10));
    auto material1 = std::make_shared<ert::mat::lambertian>(ert::color(0.5));
    w.add(ert::geo::xz_rect(-0.2, 0.2, -0.2, 0.2, 0.01, material1));
    w.add(ert::geo::xz_rect(-10, 10, -10, 10, 2, light, ert::geo::Face::BACK));
    w.add(ert::geo::poly_mesh("../../res/models/dragon.obj", ert::vec3{0.1f}, {0.0f, 0.1f, 0.0f}));
    w.commit();
}

void ajax(ert::scene &w) {
    //ground
    //auto earth_material = std::make_shared<ert::mat::lambertian>(std::make_shared<ert::tex::image_texture>("../../res/earthmap.jpeg"));
    auto material1 = std::make_shared<ert::mat::lambertian>(ert::color(0.5, 0.5, 0.7));

    w.add(ert::geo::xz_rect(-0.2, 0.2, -0.2, 0.35, 0.01, mirror));
    //back
    auto material2 = std::make_shared<ert::mat::lambertian>(ert::color(0.7, 0.5, 0.5));
    w.add(ert::geo::xy_rect(0.2, -0.2, 0.3, 0.01, -0.15, material2));
    //w.add(ert::geo::xy_rect(0.1,-0.1,0.2,0.01,-0.1,glass));
    //font
    //w.add(ert::geo::xy_rect(0.1,-0.1,0.2,0.01,0.1,glass));
    w.add(ert::geo::xy_rect(0.2, -0.2, 0.3, 0.01, 0.35, material2, ert::geo::Face::BACK));
    //left
    auto material3 = std::make_shared<ert::mat::lambertian>(ert::color(0.5, 0.7, 0.5));
    w.add(ert::geo::yz_rect(0.01, 0.3, -0.2, 0.35, -0.15, material3, ert::geo::Face::BACK));
    // w.add(ert::geo::yz_rect(0.01,0.2,-0.1,0.1,-0.1,glass, ert::geo::Face::BACK));
    //right
    w.add(ert::geo::yz_rect(0.01, 0.3, -0.2, 0.35, 0.15, material3));
    //  w.add(ert::geo::yz_rect(0.01,0.2,-0.1,0.1,0.1,glass));
    //ceiling
    w.add(ert::geo::xz_rect(-0.2, 0.2, -0.2, 0.35, 0.3, matte, ert::geo::Face::BACK));
    //  w.add(ert::geo::xz_rect(-0.1,0.1,-0.1,0.1,0.2,glass, ert::geo::Face::BACK));

    auto light = std::make_shared<ert::mat::diffuse_light>(100.0 * ert::color(0.70, 0.80, 1.00));

    //auto small_light = std::make_shared<ert::geo::xz_rect>(-0.005,0.005,-0.005,0.005,0.29999,light, ert::geo::Face::BACK);
    auto huge_light = std::make_shared<ert::geo::xz_rect>(-0.05, 0.05, -0.05, 0.05, 0.29999, light,
                                                          ert::geo::Face::BACK);

    w.addLight(huge_light);
    //w.add(ert::geo::xz_rect(-0.14,0.14,-0.2,0.35,0.25,matte, ert::geo::Face::BACK));
    //w.add(ert::geo::xz_rect(-0.14,0.14,-0.2,0.35,0.2501,matte));
    //w.add(ert::geo::xy_rect(0.04,-0.04,0.15-0.04,0.15+0.04,-0.199,light));
    //w.add(ert::geo::yz_rect(0.15-0.04,0.15+0.04,-0.04,0.04,-0.199,light, ert::geo::Face::BACK));
    //w.add(ert::geo::yz_rect(0.15-0.04,0.15+0.04,-0.04,0.04,0.199,light));

    auto aja = ert::geo::poly_mesh("../../res/models/ajax.obj", ert::vec3{0.005f}, {0.f, 0.018f, 0.1f});
    aja.set_mat(mirror);
    w.add(std::move(aja));

    w.commit();
}

float v = 0.001;

void livingroom(ert::scene &w, int t) {
    auto z0 = -0.005 + 24 * 3 * v;
    auto z1 = 0.005 + 24 * 3 * v;
    auto light = std::make_shared<ert::mat::diffuse_light>(100.0 * ert::color(0.70, 0.80, 1.00));
    auto huge_light = std::make_shared<ert::geo::xz_rect>(-0.005, 0.005, z0 - v * t, z1 - v * t, 0.15, light,
                                                          ert::geo::Face::BACK);
    w.addLight(huge_light);

    auto room = ert::geo::poly_mesh("../../res/models/room/Room #1.obj", ert::vec3{0.02f}, {0.f, 0.02f, 0.f});
    //auto house = ert::geo::poly_mesh("../../res/models/Bambo_House/Bambo_House.obj",ert::vec3{0.02f},{0.f,0.00f,0.1f});
    //room.set_mat(glass);
    w.add(std::move(room));
    w.commit();
}

void wave(ert::scene &w) {

    auto water = std::make_shared<ert::mat::dielectric>(1.33);
    auto mat1 = std::make_shared<ert::mat::lambertian>(ert::color{0.5f, 0.5f, 0.5f});
    /*
    auto N = 10;
    auto stride = 0.2f/N;
    for(int i =0;i<N;++i){
        for(int j=0;j<N;++j){
            auto random_mat =std::make_shared<ert::mat::lambertian>(ert::color(ert::random_FLOAT(),ert::random_FLOAT(),ert::random_FLOAT()));
            auto random_glass = std::make_shared<ert::mat::dielectric>(ert::random_FLOAT());
            w.add(ert::geo::xz_rect(-0.1f+i*stride,-0.1f+(i+1)*stride,-0.1f+j*stride,-0.1f+(j+1)*stride,0.02,random_glass));
        }
    }
     */
    //auto wav =  ert::geo::poly_mesh("../../res/models/subdiv_quad.obj",ert::vec3{0.1f},{0.f,0.02f,0.f});
    // wav.set_mat(water);

    // w.add(std::move(wav));

    w.add(ert::geo::xz_rect(-0.1, 0.1, -0.1, 0.1, 0.02, water));
    w.add(ert::geo::xz_rect(-0.1, 0.1, -0.1, 0.1, 0, mat1));

    //back
    w.add(ert::geo::xy_rect(0.1, -0.1, 0.05, 0.0, -0.1, mat1));
    //left
    w.add(ert::geo::yz_rect(0.0, 0.05, -0.1, 0.1, -0.1, mat1, ert::geo::Face::BACK));
    //right
    w.add(ert::geo::yz_rect(0.0, 0.05, -0.1, 0.1, 0.1, mat1));

    auto light = std::make_shared<ert::mat::diffuse_light>(10.0 * ert::color(0.70, 0.80, 1.00));

    //auto small_light = std::make_shared<ert::geo::xz_rect>(-0.005,0.005,-0.005,0.005,0.29999,light, ert::geo::Face::BACK);
    auto huge_light = std::make_shared<ert::geo::xz_rect>(-0.05, 0.05, -0.05, 0.05, 0.2, light, ert::geo::Face::BACK);
    w.addLight(huge_light);
    w.commit();
}
