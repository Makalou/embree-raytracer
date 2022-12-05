include_directories(${CMAKE_SOURCE_DIR}/include/geom)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/ext)
include_directories(${CMAKE_SOURCE_DIR}/ext/embree-concise/include)

add_library(ert_geom_obj OBJECT poly_mesh.cpp fast_obj.c sphere.cpp aarect.cpp)
