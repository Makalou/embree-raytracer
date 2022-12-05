include_directories(${CMAKE_SOURCE_DIR}/include/rend)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/ext)
include_directories(${CMAKE_SOURCE_DIR}/ext/embree-concise/include)

add_library(ert_rend_obj OBJECT renderer.cpp viewer.cpp camera.cpp scene.cpp)

find_package(TBB REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# target_compile_options(TBB::tbb INTERFACE -fno-aligned-allocation)
target_link_libraries(ert_rend_obj PUBLIC TBB::tbb OpenGL::GL glfw libglew_static embree)
