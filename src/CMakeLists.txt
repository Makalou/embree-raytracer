add_subdirectory(core)
add_subdirectory(geom)
add_subdirectory(rend)

include_directories(${CMAKE_SOURCE_DIR}/ext/embree-concise/include)
include_directories(${CMAKE_SOURCE_DIR}/ext)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(embree_raytracer main.cpp ../include/ert.h ../include/geom/sphere.h ../include/core/spectrum.h ../include/core/sampler.h ../include/rend/integrator.h ../include/rend/photon_mapping.h)

target_link_libraries(embree_raytracer PUBLIC ert_core_obj)
target_link_libraries(embree_raytracer PUBLIC ert_geom_obj)
target_link_libraries(embree_raytracer PUBLIC ert_rend_obj)
