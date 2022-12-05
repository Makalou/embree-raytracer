//
// Created by 王泽远 on 2022/4/19.
//
#include "aarect.h"

NAMESPACE_BEGIN(ert)
    NAMESPACE_BEGIN(geo)
        void xy_rect::addTo(std::shared_ptr<scene> s) {
            RTCGeometry geom1 = rtcNewGeometry(s->m_device, RTC_GEOMETRY_TYPE_TRIANGLE);
            auto *ib1 = (unsigned *) rtcSetNewGeometryBuffer(geom1,
                                                             RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
                                                             3 * sizeof(unsigned), 1);
            ib1[0] = 0;
            ib1[1] = 1;
            ib1[2] = 2;

            auto *vb1 = (float *) rtcSetNewGeometryBuffer(geom1,
                                                          RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
                                                          3 * sizeof(float), 3);

            //(x0,k,z0),(x1,k,z0),(x0,k,z1),(x1,k,z1)
            vb1[idx1] = x0;
            vb1[idx2] = y0;
            vb1[2] = k;
            vb1[3] = x1;
            vb1[4] = y0;
            vb1[5] = k;
            vb1[idx3] = x0;
            vb1[idx4] = y1;
            vb1[8] = k;

            RTCGeometry geom2 = rtcNewGeometry(s->m_device, RTC_GEOMETRY_TYPE_TRIANGLE);
            auto *ib2 = (unsigned *) rtcSetNewGeometryBuffer(geom2,
                                                             RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
                                                             3 * sizeof(unsigned), 1);
            ib2[0] = 0;
            ib2[1] = 1;
            ib2[2] = 2;

            auto *vb2 = (float *) rtcSetNewGeometryBuffer(geom2,
                                                          RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
                                                          3 * sizeof(float), 3);

            vb2[idx1] = x1;
            vb2[idx2] = y0;
            vb2[2] = k;
            vb2[3] = x1;
            vb2[4] = y1;
            vb2[5] = k;
            vb2[idx3] = x0;
            vb2[idx4] = y1;
            vb2[8] = k;


            rtcCommitGeometry(geom1);
            rtcCommitGeometry(geom2);
            auto geomID1 = rtcAttachGeometry(s->m_scene, geom1);
            auto geomID2 = rtcAttachGeometry(s->m_scene, geom2);

            //s->m_mat_table.insert({geomID1, mp});
            //s->m_mat_table.insert({geomID2, mp});
            s->m_mat_table.push_back(mp);
            s->m_mat_table.push_back(mp);

            rtcReleaseGeometry(geom1);
            rtcReleaseGeometry(geom2);
        }

        void xz_rect::addTo(std::shared_ptr<scene> s) {
            RTCGeometry geom1 = rtcNewGeometry(s->m_device, RTC_GEOMETRY_TYPE_TRIANGLE);
            auto *ib1 = (unsigned *) rtcSetNewGeometryBuffer(geom1,
                                                             RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
                                                             3 * sizeof(unsigned), 1);
            ib1[0] = 0;
            ib1[1] = 1;
            ib1[2] = 2;

            auto *vb1 = (float *) rtcSetNewGeometryBuffer(geom1,
                                                          RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
                                                          3 * sizeof(float), 3);

            //(x0,k,z0),(x1,k,z0),(x0,k,z1),(x1,k,z1)
            vb1[idx1] = x0;
            vb1[1] = k;
            vb1[idx2] = z1;
            vb1[3] = x1;
            vb1[4] = k;
            vb1[5] = z0;
            vb1[idx3] = x0;
            vb1[7] = k;
            vb1[idx4] = z0;

            RTCGeometry geom2 = rtcNewGeometry(s->m_device, RTC_GEOMETRY_TYPE_TRIANGLE);
            auto *ib2 = (unsigned *) rtcSetNewGeometryBuffer(geom2,
                                                             RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
                                                             3 * sizeof(unsigned), 1);
            ib2[0] = 0;
            ib2[1] = 1;
            ib2[2] = 2;

            auto *vb2 = (float *) rtcSetNewGeometryBuffer(geom2,
                                                          RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
                                                          3 * sizeof(float), 3);

            vb2[idx1] = x0;
            vb2[1] = k;
            vb2[idx2] = z1;
            vb2[3] = x1;
            vb2[4] = k;
            vb2[5] = z1;
            vb2[idx3] = x1;
            vb2[7] = k;
            vb2[idx4] = z0;


            rtcCommitGeometry(geom1);
            rtcCommitGeometry(geom2);
            id1 = rtcAttachGeometry(s->m_scene, geom1);
            id2 = rtcAttachGeometry(s->m_scene, geom2);
            //s->m_mat_table.insert({id1, mp});
            //s->m_mat_table.insert({id2, mp});
            spdlog::info("{},{}", id1, id2);
            s->m_mat_table.push_back(mp);
            s->m_mat_table.push_back(mp);
            mp_scene = s;
            rtcReleaseGeometry(geom1);
            rtcReleaseGeometry(geom2);
        }

        void yz_rect::addTo(std::shared_ptr<scene> s) {
            RTCGeometry geom1 = rtcNewGeometry(s->m_device, RTC_GEOMETRY_TYPE_TRIANGLE);
            auto *ib1 = (unsigned *) rtcSetNewGeometryBuffer(geom1,
                                                             RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
                                                             3 * sizeof(unsigned), 1);
            ib1[0] = 0;
            ib1[1] = 1;
            ib1[2] = 2;

            auto *vb1 = (float *) rtcSetNewGeometryBuffer(geom1,
                                                          RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
                                                          3 * sizeof(float), 3);

            //(x0,k,z0),(x1,k,z0),(x0,k,z1),(x1,k,z1)
            vb1[0] = k;
            vb1[idx1] = y0;
            vb1[idx2] = z1;
            vb1[3] = k;
            vb1[4] = y1;
            vb1[5] = z0;
            vb1[6] = k;
            vb1[idx3] = y0;
            vb1[idx4] = z0;

            RTCGeometry geom2 = rtcNewGeometry(s->m_device, RTC_GEOMETRY_TYPE_TRIANGLE);
            auto *ib2 = (unsigned *) rtcSetNewGeometryBuffer(geom2,
                                                             RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
                                                             3 * sizeof(unsigned), 1);
            ib2[0] = 0;
            ib2[1] = 1;
            ib2[2] = 2;

            auto *vb2 = (float *) rtcSetNewGeometryBuffer(geom2,
                                                          RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
                                                          3 * sizeof(float), 3);

            vb2[0] = k;
            vb2[idx1] = y0;
            vb2[idx2] = z1;
            vb2[3] = k;
            vb2[4] = y1;
            vb2[5] = z1;
            vb2[6] = k;
            vb2[idx3] = y1;
            vb2[idx4] = z0;


            rtcCommitGeometry(geom1);
            rtcCommitGeometry(geom2);
            auto geomID1 = rtcAttachGeometry(s->m_scene, geom1);
            auto geomID2 = rtcAttachGeometry(s->m_scene, geom2);
            //s->m_mat_table.insert({geomID1, mp});
            //s->m_mat_table.insert({geomID2, mp});

            s->m_mat_table.push_back(mp);
            s->m_mat_table.push_back(mp);

            rtcReleaseGeometry(geom1);
            rtcReleaseGeometry(geom2);
        }
    NAMESPACE_END(geo)
NAMESPACE_END(ert)