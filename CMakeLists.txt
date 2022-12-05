cmake_minimum_required(VERSION 3.21)
project(embree_raytracer)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(NoInSourceBuilds)

add_subdirectory(ext ext_build)
add_subdirectory(src)

