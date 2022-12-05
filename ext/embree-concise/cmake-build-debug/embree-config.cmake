## Copyright 2009-2021 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

# use default install config
INCLUDE("${CMAKE_CURRENT_LIST_DIR}/embree-config-install.cmake")

# and override path variables to match for build directory
SET(EMBREE_INCLUDE_DIRS /Users/wangzeyuan/code/embree-raytracer/ext/embree-concise/include)
SET(EMBREE_LIBRARY /Users/wangzeyuan/code/embree-raytracer/ext/embree-concise/cmake-build-debug/libembree3.3.13.3.dylib)
SET(EMBREE_LIBRARIES ${EMBREE_LIBRARY})
