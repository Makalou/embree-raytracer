add_subdirectory(embree-concise)

#build glew
option(ONLY_LIBS "" ON)
option(glew-cmake_BUILD_SHARED "" OFF)
add_subdirectory(glew)

