//
// Created by 王泽远 on 2022/4/14.
//
#include "poly_mesh.h"
#include "spdlog/spdlog.h"

NAMESPACE_BEGIN(ert)
    NAMESPACE_BEGIN(geo)
        void poly_mesh::loadAndAttachObj(std::shared_ptr<scene> s, std::string path) {
            mesh = fast_obj_read(path.c_str());
            if (mesh == nullptr) {
                spdlog::error("Unable to load OBJ file {}!\n", path);
                return;
            }
            spdlog::info("load mesh{}:\n"
                         "group count: {}\n"
                         "object count: {}\n"
                         "position count: {}\n"
                         "face count: {}\n"
                         "material count: {}\n"
                         "normal count: {}\n"
                         "texcoord count: {}\n",
                         path, mesh->group_count, mesh->object_count, mesh->position_count, mesh->face_count,
                         mesh->material_count, mesh->normal_count, mesh->texcoord_count);

            for (unsigned int ii = 0; ii < mesh->group_count; ++ii) {
                const auto &grp = mesh->groups[ii];
                if (grp.name)
                    spdlog::info("load group {}:\tface count:{} ", grp.name, grp.face_count);
                loadAndAttachGrp(grp, s);
            }
            for (int i = 0; i < mesh->material_count; ++i) {
                spdlog::info("material name : {}", mesh->materials[i].name);
            }
        }

        void poly_mesh::addTo(std::shared_ptr<scene> s) {
            loadAndAttachObj(s, m_path);
        }

        void poly_mesh::loadAndAttachGrp(const fastObjGroup &grp, std::shared_ptr<scene> s) {

            unsigned int grp_index_idx = 0;

            for (unsigned int grp_face_idx = 0; grp_face_idx < grp.face_count; ++grp_face_idx) {

                auto face_mat = mesh->materials[mesh->face_materials[grp_face_idx]];
                auto mp = m_mat ? m_mat : std::make_shared<mat::lambertian>(ert::color{0.5f});
                auto mat_name = std::string(face_mat.name);
                if (mat_name == "TVDisplay") {
                    mp = std::make_shared<mat::metal>(ert::color{1.0f}, 0);
                } else if (mat_name == "TV") {
                    mp = std::make_shared<mat::metal>(ert::color{0.1f}, 0.5);
                } else if (mat_name == "Wall_Frame") {
                    mp = std::make_shared<mat::lambertian>(ert::color{0.05f, 0.1f, 0.1f});
                } else if (mat_name == "PS4Bottom" || mat_name == "PS4Font" || mat_name == "PS4Top") {
                    mp = std::make_shared<mat::metal>(ert::color{0.0f}, 1);
                } else if (mat_name == "Wall") {
                    mp = std::make_shared<mat::lambertian>(ert::color{1.0f});
                } else if (mat_name == "joystick") {
                    mp = std::make_shared<mat::metal>(ert::color{1.0f}, 1);
                } else if (mat_name == "leaf.001") {
                    mp = std::make_shared<mat::lambertian>(ert::color{0.0f, 0.1f, 0.0f});
                } else if (mat_name == "stem") {
                    mp = std::make_shared<mat::lambertian>(ert::color{0.0f, 0.2f, 0.0f});
                } else if (mat_name == "sand") {
                    mp = std::make_shared<mat::lambertian>(ert::color{0.1f, 0.1f, 0.1f});
                } else if (mat_name == "Cabinet" || mat_name == "Tea_Table_" || mat_name == "Table_Drawer" ||
                           mat_name == "Drawer_Holder") {
                    mp = std::make_shared<mat::lambertian>(
                            ert::color{222.0f / 255.0f, 184.0f / 255.0f, 135.0f / 255.0f});
                } else if (mat_name == "Mat") {
                    mp = std::make_shared<mat::lambertian>(ert::color{1.0f, 0.0f, 0.0f});
                } else if (mat_name == "Mat.6") {
                    mp = std::make_shared<mat::lambertian>(ert::color{1.0f, 0.0f, 0.0f});
                } else if (mat_name == "BookCover") {
                    mp = std::make_shared<mat::lambertian>(ert::color{0.1f, 0.0f, 0.0f});
                } else if (mat_name == "TVBacklight") {
                    mp = std::make_shared<mat::diffuse_light>(ert::color{10.0f, 9.0f, 9.0f});
                } else if (mat_name == "Sofa") {
                    mp = std::make_shared<mat::lambertian>(
                            ert::color{140.0f / 255.0f, 120.0f / 255.0f, 115.0f / 255.0f});
                } else if (mat_name == "Mug_Inner" || mat_name == "MugOuter") {
                    mp = std::make_shared<mat::lambertian>(ert::color{1.0f, 0.0f, 0.0f});
                } else if (mat_name == "DVD_Player") {
                    mp = std::make_shared<mat::lambertian>(ert::color{0.0f, 0.0f, 0.0f});
                } else if (mat_name == "gorshok") {
                    mp = std::make_shared<mat::lambertian>(ert::color{1.0f, 0.9f, 1.0f});
                } else if (mat_name == "Water") {
                    mp = std::make_shared<ert::mat::dielectric>(1.5);
                } else if (mat_name == "Guitar") {
                    mp = std::make_shared<mat::lambertian>(ert::color{111.0f / 255.0f, 92.0f / 255.0f, 67.5f / 255.0f});
                }

                auto fv = mesh->face_vertices[grp.face_offset + grp_face_idx];

                for (unsigned int fv_idx = 3, ii = grp_index_idx + 1; fv_idx <= fv; fv_idx++) {
                    auto mi0 = mesh->indices[grp.index_offset + grp_index_idx];

                    RTCGeometry geom = rtcNewGeometry(s->m_device, RTC_GEOMETRY_TYPE_TRIANGLE);

                    auto *ib = (unsigned *) rtcSetNewGeometryBuffer(geom,
                                                                    RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
                                                                    3 * sizeof(unsigned), 1);
                    ib[0] = 0;
                    ib[1] = 1;
                    ib[2] = 2;
                    auto *vb = (float *) rtcSetNewGeometryBuffer(geom,
                                                                 RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
                                                                 3 * sizeof(float), 3);

                    vb[0] = m_scale.x() * mesh->positions[3 * mi0.p] + m_transform.x();
                    vb[1] = m_scale.y() * mesh->positions[3 * mi0.p + 1] + m_transform.y();
                    vb[2] = m_scale.z() * mesh->positions[3 * mi0.p + 2] + m_transform.z();

                    for (unsigned int v = 1; v < 3; ++v) {

                        auto mi = mesh->indices[grp.index_offset + ii];

                        if (mi.p) {
                            vb[3 * v] = m_scale.x() * mesh->positions[3 * mi.p] + m_transform.x();
                            vb[3 * v + 1] = m_scale.y() * mesh->positions[3 * mi.p + 1] + m_transform.y();
                            vb[3 * v + 2] = m_scale.z() * mesh->positions[3 * mi.p + 2] + m_transform.z();
                        }
                        ii++;
                    }
                    ii -= 1;

                    rtcCommitGeometry(geom);
                    auto geomID = rtcAttachGeometry(s->m_scene, geom);
                    rtcReleaseGeometry(geom);

                    auto m_key = s->m_materials.find(mp);
                    if (m_key != s->m_materials.cend()) {
                        //s->m_mat_table.insert({geomID, *m_key});
                        s->m_mat_table.push_back(*m_key);
                    } else {
                        s->m_materials.insert(mp);
                        //s->m_mat_table.insert({geomID, mp});
                        s->m_mat_table.push_back(mp);
                    }
                }

                grp_index_idx += fv;
            }

        }
    NAMESPACE_END(geo)
NAMESPACE_END(ert)

