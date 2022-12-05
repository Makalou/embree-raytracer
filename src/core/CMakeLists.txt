include_directories(${CMAKE_SOURCE_DIR}/include/core)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/ext)
add_library(ert_core_obj OBJECT color.cpp texture.cpp sampler.cpp)